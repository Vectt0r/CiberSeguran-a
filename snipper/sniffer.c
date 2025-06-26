#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include "windivert.h"

#define MAX_PACKET_SIZE 0xFFFF

// Estrutura IP simplificada
typedef struct {
    UINT8  ver_ihl;
    UINT8  tos;
    UINT16 length;
    UINT16 id;
    UINT16 frag_off;
    UINT8  ttl;
    UINT8  protocol;
    UINT16 checksum;
    UINT32 src_addr;
    UINT32 dst_addr;
} IPV4HDR;

// Estruturas TCP/UDP mínimas
typedef struct {
    UINT16 src_port;
    UINT16 dst_port;
} TCPHDR;

typedef struct {
    UINT16 src_port;
    UINT16 dst_port;
} UDPHDR;

int main() {
    HANDLE handle;
    WINDIVERT_ADDRESS addr;
    char packet[MAX_PACKET_SIZE];
    UINT packet_len;

    // Abrir filtro para pacotes TCP e UDP (camada de rede)
    handle = WinDivertOpen("ip and (tcp or udp)", WINDIVERT_LAYER_NETWORK, 0, 0);
    if (handle == INVALID_HANDLE_VALUE) {
        printf("Erro ao abrir WinDivert. Execute como administrador. Código: %d\n", GetLastError());
        return 1;
    }

    printf("Sniffer iniciado. Capturando pacotes TCP/UDP...\n");

    while (1) {
        if (!WinDivertRecv(handle, packet, sizeof(packet), &packet_len, &addr)) {
            continue;
        }

        // Interpretar cabeçalho IP
        IPV4HDR *ip_header = (IPV4HDR *)packet;
        UINT8 ip_header_len = (ip_header->ver_ihl & 0x0F) * 4;

        // Converter IPs para string
        char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &ip_header->src_addr, src_ip, sizeof(src_ip));
        inet_ntop(AF_INET, &ip_header->dst_addr, dst_ip, sizeof(dst_ip));

        // TCP
        if (ip_header->protocol == IPPROTO_TCP) {
            TCPHDR *tcp = (TCPHDR *)(packet + ip_header_len);
            printf("[TCP] %s:%d → %s:%d\n", src_ip, ntohs(tcp->src_port), dst_ip, ntohs(tcp->dst_port));
        }
        // UDP
        else if (ip_header->protocol == IPPROTO_UDP) {
            UDPHDR *udp = (UDPHDR *)(packet + ip_header_len);
            printf("[UDP] %s:%d → %s:%d\n", src_ip, ntohs(udp->src_port), dst_ip, ntohs(udp->dst_port));
        }
    }

    WinDivertClose(handle);
    return 0;
}
