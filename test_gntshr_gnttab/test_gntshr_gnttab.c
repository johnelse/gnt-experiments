#include <stdio.h>
#include <stdlib.h>

#include <xenctrl.h>

void *share_pages(
        xc_gntshr *gntshr_interface,
        uint32_t domid,
        int count,
        uint32_t *refs) {
    void *share;

    share = xc_gntshr_share_pages(gntshr_interface, domid, count, refs, 0);

    return share;
}

int main(int argc, char **argv) {
    xc_gntshr *gntshr_if;
    xc_gnttab *gnttab_if;

    void* share;

    gntshr_if = xc_gntshr_open(NULL, 0);
    if(!gntshr_if) {
        printf("Failed to open gntshr interface.\n");
        goto Error;
    }
    gnttab_if = xc_gnttab_open(NULL, 0);
    if(!gnttab_if) {
        printf("Failed to open gnttab interface.\n");
        goto Error;
    }

    if(xc_gntshr_close(gntshr_if)) {
        printf("Failed to close gntshr interface.\n");
        goto Error;
    }
    if(xc_gnttab_close(gnttab_if)) {
        printf("Failed to close gnttab interface.\n");
        goto Error;
    }

    exit(0);

Error:
    exit(1);
}
