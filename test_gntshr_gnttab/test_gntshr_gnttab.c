#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include <xenctrl.h>

#define PAGE_COUNT 1
#define DOMID 0

/* Open one interface to each of gntshr and gnttab, and use these for both
 * mapping and unmapping. */
int test_open_once() {
    xc_gntshr *gntshr_if;
    xc_gnttab *gnttab_if;

    uint32_t refs[PAGE_COUNT];
    uint32_t domids[PAGE_COUNT];
    void* local_share;
    void* remote_share;

    int i;
    int err;

    gntshr_if = xc_gntshr_open(NULL, 0);
    if(!gntshr_if) {
        printf("Failed to open gntshr interface.\n");
        return 1;
    }
    gnttab_if = xc_gnttab_open(NULL, 0);
    if(!gnttab_if) {
        printf("Failed to open gnttab interface.\n");
        return 1;
    }

    local_share = xc_gntshr_share_pages(gntshr_if, DOMID, PAGE_COUNT, refs, 0);
    if(!local_share) {
        printf("Failed to share memory.\n");
        return 1;
    }

    for(i = 0; i < PAGE_COUNT; i++) {
        printf("Sharing page with ref %d.\n", refs[i]);
    }

    for(i = 0; i < PAGE_COUNT; i++) {
        domids[i] = DOMID;
    }
    remote_share = xc_gnttab_map_grant_refs(
            gnttab_if,
            PAGE_COUNT,
            domids,
            refs,
            PROT_READ);
    if(!remote_share) {
        printf("Failed to map memory.\n");
        return 1;
    }

    err = xc_gnttab_munmap(gnttab_if, remote_share, 1);
    if(err) {
        printf("Failed to unmap memory - got error code %d.\n", err);
        return 1;
    }

    err = xc_gntshr_munmap(gntshr_if, local_share, 1);
    if(err) {
        printf("Failed to unshare memory - got error code %d.\n", err);
        return 1;
    }

    if(xc_gntshr_close(gntshr_if)) {
        printf("Failed to close gntshr interface.\n");
        return 1;
    }
    if(xc_gnttab_close(gnttab_if)) {
        printf("Failed to close gnttab interface.\n");
        return 1;
    }

    return 0;
}

/* Open an interface to gntshr or gnttab each time it is needed. */
int test_open_multiple() {
    xc_gntshr *gntshr_if;
    xc_gnttab *gnttab_if;

    uint32_t refs[PAGE_COUNT];
    uint32_t domids[PAGE_COUNT];
    void* local_share;
    void* remote_share;

    int i;
    int err;

    gntshr_if = xc_gntshr_open(NULL, 0);
    if(!gntshr_if) {
        printf("Failed to open gntshr interface.\n");
        return 1;
    }
    gnttab_if = xc_gnttab_open(NULL, 0);
    if(!gnttab_if) {
        printf("Failed to open gnttab interface.\n");
        return 1;
    }

    local_share = xc_gntshr_share_pages(gntshr_if, DOMID, PAGE_COUNT, refs, 0);
    if(!local_share) {
        printf("Failed to share memory.\n");
        return 1;
    }

    for(i = 0; i < PAGE_COUNT; i++) {
        printf("Sharing page with ref %d.\n", refs[i]);
    }

    for(i = 0; i < PAGE_COUNT; i++) {
        domids[i] = DOMID;
    }
    remote_share = xc_gnttab_map_grant_refs(
            gnttab_if,
            PAGE_COUNT,
            domids,
            refs,
            PROT_READ);
    if(!remote_share) {
        printf("Failed to map memory.\n");
        return 1;
    }

    if(xc_gntshr_close(gntshr_if)) {
        printf("Failed to close gntshr interface.\n");
        return 1;
    }
    if(xc_gnttab_close(gnttab_if)) {
        printf("Failed to close gnttab interface.\n");
        return 1;
    }

    gntshr_if = xc_gntshr_open(NULL, 0);
    if(!gntshr_if) {
        printf("Failed to open gntshr interface.\n");
        return 1;
    }
    gnttab_if = xc_gnttab_open(NULL, 0);
    if(!gnttab_if) {
        printf("Failed to open gnttab interface.\n");
        return 1;
    }

    err = xc_gnttab_munmap(gnttab_if, remote_share, 1);
    if(err) {
        printf("Failed to unmap memory - got error code %d.\n", err);
        return 1;
    }

    err = xc_gntshr_munmap(gntshr_if, local_share, 1);
    if(err) {
        printf("Failed to unshare memory - got error code %d.\n", err);
        return 1;
    }

    if(xc_gntshr_close(gntshr_if)) {
        printf("Failed to close gntshr interface.\n");
        return 1;
    }
    if(xc_gnttab_close(gnttab_if)) {
        printf("Failed to close gnttab interface.\n");
        return 1;
    }

    return 0;
}

int main(int argc, char **argv) {
    int err;

    err = test_open_once();
    if(err) {
        return 1;
    }

    err = test_open_multiple();
    if(err) {
        return 1;
    }


    return 0;
}
