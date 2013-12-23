#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include <xenctrl.h>

int main(int argc, char **argv) {
    xc_gntshr *gntshr_if;
    xc_gnttab *gnttab_if;

    uint32_t *refs;
    void* local_share;
    void* remote_share;

    int err;

    gntshr_if = xc_gntshr_open(NULL, 0);
    if(!gntshr_if) {
        printf("Failed to open gntshr interface.\n");
        exit(1);
    }
    gnttab_if = xc_gnttab_open(NULL, 0);
    if(!gnttab_if) {
        printf("Failed to open gnttab interface.\n");
        exit(1);
    }

    refs = malloc(sizeof(uint32_t));
    local_share = xc_gntshr_share_pages(gntshr_if, 0, 1, refs, 0);
    if(!local_share) {
        printf("Failed to share memory.\n");
        exit(1);
    }

    remote_share = xc_gnttab_map_grant_ref(gnttab_if, 0, *refs, PROT_READ);
    if(!remote_share) {
        printf("Failed to map memory.\n");
        exit(1);
    }

    err = xc_gnttab_munmap(gnttab_if, remote_share, 1);
    if(err) {
        printf("Failed to unmap memory - got error code %d.\n", err);
    }

    err = xc_gntshr_munmap(gntshr_if, local_share, 1);
    if(err) {
        printf("Failed to unshare memory - got error code %d.\n", err);
    }

    if(xc_gntshr_close(gntshr_if)) {
        printf("Failed to close gntshr interface.\n");
    }
    if(xc_gnttab_close(gnttab_if)) {
        printf("Failed to close gnttab interface.\n");
    }

    exit(0);
}
