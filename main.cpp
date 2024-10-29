#include <iostream>
#include <mqtt/async_client.h>
#include <mariadb/mysql.h>

using namespace std;


const string ADDRESS = "tcp://172.30.1.93:1883";
const string CLIENT_ID = "tester_client";
const string TOPIC = "home/temperature";
const string RESPONSE_TOPIC = "main2";
const string RESPONSE_MESSAGE = "Message has been received";


class Broker: public virtual mqtt::callback {
    public:
       Broker(mqtt::async_client& cli): client(cli) {}
       void connection_lost(const string& cause) override {
         cout << "Connection lost" << cause << endl;           
     }

     void message_arrived(mqtt::const_message_ptr msg) override {
      //  publish_message();
        if(msg->get_topic() == TOPIC) {
            std::cout << "Message arrived. Topic: " << msg->get_topic()
                  << ", Payload: " << msg->to_string() << std::endl;

        }else {
            cout << "Message arrived on unexpected topic: " << msg->get_topic()
                 << ", Payload: " << msg->to_string() << std::endl;
 
        }
    }

    void delivery_complete(mqtt::delivery_token_ptr token) override {}

    private:
    mqtt::async_client& client;
   
    void publish_message() {
        cout << "Entering to the this function" << endl;
        mqtt::message_ptr pubmsg = mqtt::make_message(RESPONSE_TOPIC, RESPONSE_MESSAGE);
        pubmsg->set_qos(0);
        try {
            client.publish(pubmsg)->wait();
            cout << "Message Published to " << RESPONSE_TOPIC << endl;
        } catch (const mqtt::exception& e) {
            std::cerr << "Publish Error: " << e.what() << std::endl;
        }
    }
};


int main() {
    
    mqtt::async_client client(ADDRESS, CLIENT_ID);
    Broker callback(client);
    client.set_callback(callback);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(60);
    connOpts.set_password("this is gonna be password");
    connOpts.set_user_name("another user name");

    try {
        cout << "Connecting to the Server" << endl;
        client.connect(connOpts)->wait();
        cout << "Connected" << endl;
        mqtt::message_ptr msg = mqtt::make_message(TOPIC, RESPONSE_MESSAGE);
        msg->set_qos(1);
        client.publish(msg)->wait();
        client.subscribe(TOPIC, 1)-> wait();
        client.subscribe("main/*", 2)-> wait();
        cout << "Subscribed" << endl;


        cout << "Listening Incoming messages" << endl;

        while (true) {

        }

    }catch(const mqtt::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;

}