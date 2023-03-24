#ifndef KCP_SESS_H
#define KCP_SESS_H

#include "fec.h"
#include <sys/types.h>
#include <stdint.h>
typedef struct IKCPCB ikcpcb;
class UDPSession  {
private:
    int m_sockfd{0};
    ikcpcb *m_kcp{nullptr};
    byte m_buf[2048];
    byte m_streambuf[65535];
    size_t m_streambufsiz{0};

    FEC fec;
    uint32_t pkt_idx{0};
    std::vector<row_type> shards;
    size_t dataShards{0};
    size_t parityShards{0};
public:
    // Dial connects to the remote server and returns UDPSession.
    static UDPSession *Dial(const char *ip, uint16_t port);

    // DialWithOptions connects to the remote address "raddr" on the network "udp" with packet encryption
    static UDPSession *DialWithOptions(const char *ip, uint16_t port, size_t dataShards, size_t parityShards);

    // Update will try reading/writing udp packet, pass current unix millisecond
    void Update(uint32_t current) noexcept;

    // Destroy release all resource related.
    static void Destroy(UDPSession *sess);

    // Read reads from kcp with buffer empty sz.
    int Read(char *buf, size_t sz) noexcept;

    // Write writes into kcp with buffer empty sz.
    int Write(const char *buf, size_t sz) noexcept;

    // Set DSCP value
    int SetDSCP(int dscp) noexcept;

    // SetStreamMode toggles the stream mode on/off
    void SetStreamMode(bool enable) noexcept;

    // Wrappers for kcp control
    int NoDelay(int nodelay, int interval, int resend, int nc);

    int WndSize(int sndwnd, int rcvwnd);

    int SetMtu(int mtu);

private:
    UDPSession() = default;
    ~UDPSession() = default;

    UDPSession(const UDPSession &) = delete;
    UDPSession &operator=(const UDPSession &) = delete;

    // DialIPv6 is the ipv6 version of Dial.
    static UDPSession *dialIPv6(const char *ip, uint16_t port);

    // out_wrapper
    static int out_wrapper(const char *buf, int len, struct IKCPCB *kcp, void *user);

    // output udp packet
    int output(const void *buffer, size_t length);

    static UDPSession *createSession(int sockfd);


};

uint32_t currentMs();
#ifdef _WIN32
void usleep(int64_t t);
#endif
#endif //KCP_SESS_H
