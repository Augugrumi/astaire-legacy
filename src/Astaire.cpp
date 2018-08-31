/*
 ============================================================================
 Name        : Astaire.cpp
 Author      : Marco Zanella, Davide Polonio
 Version     :
 Copyright   : GPLv3+
 Description : Hello World in C++,
 ============================================================================
 */

#include <iostream>
#include <thread>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <csignal>
#include <vector>
#include <viface/viface.hpp>
#include <tins/tins.h>
#include <boost/log/trivial.hpp>
#include <boost/version.hpp>
#include <boost/shared_ptr.hpp>
#include <unistd.h>
#include <sys/types.h>

#include "config.h"
#include "connection/ConnectionManager.h"


using namespace std;

// Atomic boolean to determine if we need to exit the application
volatile sig_atomic_t quit = 0;


// Signal handler routine
void signal_handler(int signal)
{
    switch (signal) {
        case SIGINT:
            quit = 1;
            break;
    }
}

// Packet reception dispatcher
class MyDispatcher
{
    private:

        int count = 0;

    public:

        bool handler(string const& name, uint id, vector<uint8_t>& packet) {
            cout << "+++ Received packet " << dec << count;
            cout << " from interface " << name;
            cout << " (" << id << ") of size " << packet.size();
            this->count++;

            // Continue processing only if quit wasn't requested
            return quit == 0;
        }
};

// Receive worker function
void receive_wkr(viface::VIface* iface)
{
    set<viface::VIface*> ifaces = {iface};
    MyDispatcher printer;
    viface::dispatcher_cb mycb = bind(
        &MyDispatcher::handler,
        &printer,
        placeholders::_1,
        placeholders::_2,
        placeholders::_3
        );

    while (!quit) {
        // We configure a dispatcher with a timeout of 1 second
        viface::dispatch(ifaces, mycb, 1000);
    }
}

// Send worker function
void send_wkr(viface::VIface* iface, vector<uint8_t> pkt)
{
    while (!quit) {
        iface->send(pkt);
        this_thread::sleep_for(chrono::seconds(1));
    }
}

/**
 * This example shows how to use threads to handle both IO tasks of a virtual
 * interface: send and receive packets.
 *
 * We setup a signal handler to catch the SIGINT signal (Ctrl+C) that will
 * request both threads to stop. Those threads will poll a flag to terminate.
 *
 * Sadly, signals doesn't propagate to child threads, and thus select() and
 * sleep() calls don't get interrupted, and thus polling is required :/
 */

typedef boost::shared_ptr<connection::ConnectionManager> SmartConn;

int main(int argc, const char* argv[])
{

	BOOST_LOG_TRIVIAL(info) << "Spinning up " << PACKAGE_STRING << "...";

#if BOOST_VERSION >= 106600
	BOOST_LOG_TRIVIAL(info) << "Using a version of boost newer then 1.66!";
#endif

    signal(SIGINT, signal_handler);

    // Returned is uid_t, that's an unsigned int
    if (getuid() != 0) {
    	BOOST_LOG_TRIVIAL(fatal) << PACKAGE_NAME << " must run as root. Exiting";
    	exit(1);
    }

    SmartConn connection = SmartConn(
    		new connection::ConnectionManager("vnf0", "127.0.0.1")
    );

    /*try {
        // Create and bring-up interface
        viface::VIface iface("test");
        iface.setIPv4("127.0.0.1");
        if (!iface.isUp())
        	iface.up();

        // Setup signal handling to stop execution

        Tins::EthernetII pkt = \
                    Tins::EthernetII(Tins::EthernetII::BROADCAST, iface.getMAC()) /
                    Tins::IP(argv[2], argv[1]) /
                    Tins::TCP(13, 15) /
                    Tins::RawPDU("I'm a payload!");

        cout << "created pkt" << endl;

		vector<uint8_t> raw = pkt.serialize();

        // Create reception thread
        thread thr_r(receive_wkr, &iface);

        // Create sending thread
        thread thr_s(send_wkr, &iface, raw);

        // Wait for both of them to finish
        cout << "* Threads started... Ctrl+C to exit." << endl;
        thr_r.join();
        thr_s.join();
        cout << "* Welcome back to the main thread..." << endl;

        // Print final statistics and exit
        set<string> stats_set = iface.listStats();

        // Let create a list and order it just to output
        vector<string> stats(stats_set.size());
        stats.assign(stats_set.begin(), stats_set.end());
        sort(stats.begin(), stats.end());

        // Print statistics before quitting
        cout << "* Statistics:" << endl;
        for (auto & key : stats) {
            cout << "    " << key << " : ";
            cout << dec << iface.readStat(key) << endl;
        }

        cout << "* Bye!" << endl;
    } catch(exception const & ex) {
        cerr << ex.what() << endl;
        return -1;
    }*/

    return 0;
}
