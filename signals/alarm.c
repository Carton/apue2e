#include "apue.h"

static void sig_usr1(int signo)
{
	printf("Sig handler sig_usr1: caught signal %d\n", signo);
}

static void sig_alarm(int signo)
{
	printf("Sig handler sig_alarm: caught signal %d\n", signo);
}

static void sig_common(int signo)
{
	printf("Sig handler sig_common: caught signal %d\n", signo);
}

int main(void)
{
	sigset_t newmask, oldmask;
	struct sigaction ignore, common, old;
	int i;

	if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
		err_sys("Can't catch SIGUSR1");

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR2);
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("SIG_BLOCK error");

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("SIG_BLOCK error");

	if (sigismember(&oldmask, SIGUSR2))
		printf("SIGUSR2 blocked successfully\n");

	if (signal(SIGALRM, sig_alarm) == SIG_ERR)
		err_sys("Can't catch SIGALRM");

	/* Set common sig handler */
	common.sa_handler = sig_common;
	sigemptyset(&common.sa_mask); /* This is the mask when sig handler is called */
	common.sa_flags = 0;
	for (i = 1; i < 31; i++) {
		if (sigaction(i, &common, &old) == SIG_ERR) {
			printf("Can't set common sigaction for signal %d\n", i);
			continue;
		}
	}

	/* Set ignore sig handler */
	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask); /* This is the mask when sig handler is called */
	ignore.sa_flags = 0;
	for (i = 31; i < SIGRTMAX; i++) { /* This can't work */ 
		if (sigaction(i, &ignore, &old) < 0 ) {
			printf("Can't set ignore sigaction for signal %d\n", i);
			continue;
		}
	}

	/* Try raise signal to itself */
	raise(SIGUSR1);
	kill(getpid(), SIGQUIT);

	/* Since sleep() also uses SIGALRM signal, this will intefere with SIGALRM */
	for (i = 0; ;i++) {
		printf("%d\n", i);
		sleep(1);
	}
	/* Test signal impact on lock */
}

