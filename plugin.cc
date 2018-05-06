#include <iostream>
#include "plugin.h"
roc_send_func *plugin_send;
extern "C" {
void connect_handler(roc_link *link, void *custom_data)
{
    ROC_LOG_INFO("echo tcp connected\n");
    link->next_plugin_level++;
    if (link->svr->plugin[link->next_plugin_level].level != -1)
    {
        link->svr->plugin[link->next_plugin_level]
            .connect_handler(link, custom_data);
    }
    else
    {
        link->next_plugin_level = 0;
    }
}

void recv_handler(roc_link *link, void *custom_data)
{
    ROC_LOG_INFO("echo tcp recv\n");
    int len;
    char *data;
    if (!custom_data)
    {
        len = link->ibuf->tail - link->ibuf->head;
        if (len == 0)
        {
            return;
        }
        data = (char *)malloc(len + 1);
        if (!data)
        {
            return;
        }
        *(data + len) = '\0';
        roc_ringbuf_read(link->ibuf, data, len);
    }
    else
    {
        data = (char *)custom_data;
        len = strlen(data);
    }

    ROC_LOG_INFO("recv data from fd:%d, addr:%s:%d\ndata:%s\n",
                 link->fd, link->ip, link->port, data);
    plugin_send(link, data, len);
    if (!custom_data)
    {
        free(data);
    }
    link->next_plugin_level++;
    if (link->svr->plugin[link->next_plugin_level].level != -1)
    {

        link->svr->plugin[link->next_plugin_level]
            .recv_handler(link, NULL);
    }
    else
    {
        link->next_plugin_level = 0;
    }
}

void close_handler(roc_link *link, void *custom_data)
{
    ROC_LOG_INFO("echo tcp close\n");
    link->next_plugin_level++;
    if (link->svr->plugin[link->next_plugin_level].level != -1)
    {
        link->svr->plugin[link->next_plugin_level]
            .close_handler(link, custom_data);
    }
    else
    {
        link->next_plugin_level = 0;
    }
}

void init_handler(roc_svr *svr, void *custom_data)
{
    log = svr->log;
    plugin_send = svr->send;
    plugin_log_level = ROC_LOG_LEVEL_DEBUG;
    ROC_LOG_INFO("echo svr inited:%d\n", svr->next_plugin_level);
    svr->next_plugin_level++;
    if (svr->plugin[svr->next_plugin_level].level != -1)
    {
        svr->plugin[svr->next_plugin_level]
            .init_handler(svr, custom_data);
    }
    else
    {
        svr->next_plugin_level = 0;
    }
}

void fini_handler(roc_svr *svr, void *custom_data)
{
    ROC_LOG_STDERR("echo svr finied\n");
    svr->next_plugin_level++;
    if (svr->plugin[svr->next_plugin_level].level != -1)
    {
        svr->plugin[svr->next_plugin_level]
            .fini_handler(svr, custom_data);
    }
    else
    {
        svr->next_plugin_level = 0;
    }
}
}