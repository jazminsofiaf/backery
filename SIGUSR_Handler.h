#ifndef SIGUSR_HANDLER_H_
#define SIGUSR_HANDLER_H_

#include <signal.h>
#include <assert.h>

#include "EventHandler.h"

class SIGUSR_Handler : public EventHandler {

	private:
		sig_atomic_t gracefulQuit;

	public:

		SIGUSR_Handler(): gracefulQuit(0) {}

		~SIGUSR_Handler() {}

		virtual int handleSignal( int signum ) {
			assert ( signum == SIGUSR1 );
			this->gracefulQuit = 1;
			return 0;
		}

		sig_atomic_t getGracefulQuit() const {
			return this->gracefulQuit;
		}

};

#endif /* SIGINT_HANDLER_H_ */
