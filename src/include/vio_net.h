#ifndef VIO_NET_H
#define VIO_NET_H

#include "vio_types.h"
#include "vio_transport.h"

/**
 * Network Device API.
 */

struct vio_net_device;

vio_status_t vio_net_init(struct vio_net_device **out_dev, struct vio_device *base_dev);
vio_status_t vio_net_tx(struct vio_net_device *dev, const void *pkt, uint32_t len);
vio_status_t vio_net_rx(struct vio_net_device *dev, void *buf, uint32_t *len);

#endif /* VIO_NET_H */
