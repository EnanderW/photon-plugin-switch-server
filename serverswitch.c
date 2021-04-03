#include <string.h>
#include <uv.h>
#include <phapi/phapi.h>

#define is(x, b) strcmp(x, b) == 0

typedef struct switch_handle_s {
    char username[17];
    mc_server *server;
} switch_handle;

static void async_callback(uv_async_t *async) {
    switch_handle *handle = async->data;
    rlock_players();
    player *player = get_player_s(handle->username);
    runlock_players();
    if (player != NULL)
        connect_server(player, handle->server);

    free(handle);
    free(async);
}

static void on_switch_message(event_bridge_message *event) {
    if (is(event->packet_name, "switch_server")) {
        int index = 0;

        char username[17];
        unsigned char username_length = read_u_byte(event->data, &index);
        read_str_b(event->data, &index, username_length, username);

        char servername[30];
        unsigned char servername_length = read_u_byte(event->data, &index);
        read_str_b(event->data, &index, servername_length, servername);

        mc_server *server = get_server(servername);
        if (server == NULL) return;

        rlock_players();
        player *player = get_player_s(username);
        if (player == NULL || player->server == server) {
            runlock_players();
            return;
        }

        uv_loop_t *loop = player->loop;
        worker_context *player_ctx =player->worker_context;
        runlock_players();

        if (loop) {
            if (event->ctx->worker_context == player_ctx) {
                connect_server(player, server);
            } else {
                switch_handle *handle = malloc(sizeof(switch_handle));
                memcpy(handle->username, username, username_length);
                handle->server = server;

                uv_async_t *event = malloc(sizeof(uv_async_t));
                uv_async_init(loop, event, async_callback);
                event->data = handle;

                uv_async_send(event);
            }
        }
    }
}

void init_serverswitch() {
    add_listener(EVENT_BRIDGE_MESSAGE, (listener) on_switch_message);
    puts("Loaded server switch plugin!");
}