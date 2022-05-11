#include "mqtt.h"

#define CLIENT_ID "Client_ID"
#define MQTT_IP "172.16.206.200"
#define MQTT_PORT 1883
#define MQTT_TOPIC "zigbee2mqtt/0x00124b002342c261/get"

int main(int argc, char *argv[]){
    class mqtt_client *iot_client
    int rc;

    char client_id[] = CLIENT_ID;
    char host[] = MQTT_IP;
    int port = MQTT_PORT;

    mosqpp::lib_init();

    if(argc > 1){
        strcpy(host, argv[1]);
    }

    iot_client = new mqtt_client(client_id, host, port);

    while(1){
        rc = iot_client->loop();
        if(rc){
            iot_client->reconnect();
        }
        else{
            iot_client->subscribe(NULL, MQTT_TOPIC);
        }
    }

    mosqpp::cleanup();

    return 0;
}