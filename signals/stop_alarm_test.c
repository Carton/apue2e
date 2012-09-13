#include "apue.h"
int stop = 0;

static void sig_stop(int signo)
{
	printf("Sig handler sig_stop: caught signal %d\n", signo);
	stop = 1;
}

static void sig_alarm(int signo)
{
	printf("Sig handler sig_alarm: caught signal %d\n", signo);
}

int main(void)
{
	sigset_t newmask, oldmask;
	struct sigaction ignore, stop_act, old;
	int i;

	/* Set stop_act sig handler */
	stop_act.sa_handler = sig_stop;
	sigemptyset(&stop_act.sa_mask); /* This is the mask when sig handler is called */
	stop_act.sa_flags = 0;
	if (sigaction(SIGINT, &stop_act, &old) == SIG_ERR) {
		printf("Can't set stop_act sigaction for signal %d\n", SIGINT);
		exit(-1);
	}

	/* Since sleep() also uses SIGALRM signal, this will intefere with SIGALRM */
	for (i = 0; ;i++) {
		printf("%d\n", i);
		sleep(5);
		if (stop) {
			printf("stop set to 1 by SIGINT\n");
			break;
		}
	}
}

