#ifndef _WIN32
#include <unistd.h>
#endif
#include <cstring>
#include <cstdio>
#include "sess.h"
#include <assert.h>

int main() {
    srand(currentMs());

    UDPSession *sess = UDPSession::DialWithOptions("127.0.0.1", 9999, 2,2);
    sess->NoDelay(1, 20, 2, 1);
    sess->WndSize(128, 128);
    sess->SetMtu(1400);
    sess->SetStreamMode(true);
    sess->SetDSCP(46);

    assert(sess != nullptr);
    int nsent = 0;
    int nrecv = 0;
    char *buf = (char *) malloc(128);

    for (int i = 0; i < 10; i++) {
        sprintf(buf, "message:%d", i);
        auto sz = strlen(buf);
        sess->Write(buf, sz);
        sess->Update(currentMs());
        memset(buf, 0, 128);
        int n = 0;
        do {
            n = sess->Read(buf, 128);
            if (n > 0) { printf("%s\n", buf); }
            usleep(33000);
            sess->Update(currentMs());
        } while(n==0);
    }

    UDPSession::Destroy(sess);
}


