#include <iostream>
#include "plugin.h"
roc_send_func *plugin_send;
extern "C" {
void connect_handler(roc_link *link)
{
    ROC_LOG_INFO("new connection, fd:%d, addr:%s:%d\n",
                 link->fd, link->ip, link->port);
}

void recv_handler(roc_link *link)
{
    int len = link->ibuf->tail - link->ibuf->head;
    if (len == 0)
    {
        return;
    }
    char *data = (char *)malloc(len + 1);
    if (!data)
    {
        return;
    }
    *(data + len) = '\0';
    roc_ringbuf_read(link->ibuf, data, len);
    ROC_LOG_INFO("recv data from fd:%d, addr:%s:%d\ndata:%s\n",
                 link->fd, link->ip, link->port, data);
    plugin_send(link, data, len);
    free(data);
}

void close_handler(roc_link *link)
{
    ROC_LOG_WARN("link close, fd:%d, addr:%s:%d\n",
                 link->fd, link->ip, link->port);
}

void init_handler(roc_svr *svr)
{
    log = svr->log;
    plugin_send = svr->send;
    plugin_log_level = ROC_LOG_LEVEL_DEBUG;
    ROC_LOG_STDERR("svr inited\n");
}

void fini_handler(roc_svr *svr)
{
    ROC_LOG_STDERR("svr finied\n");
}
}