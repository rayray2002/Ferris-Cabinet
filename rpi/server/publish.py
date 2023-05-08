import time

import psutil
import paho.mqtt.client as mqtt
from dotenv import dotenv_values


def main(env, topic):
    print(f"Publishing to {topic}")
    
    # Establish connection to mqtt broker
    client = mqtt.Client()
    client.connect(host=env["MQTT_HOST"], port=int(env["MQTT_PORT"]))
    client.loop_start()

    try:
        while True:
            payload = input("Enter payload: ")
            client.publish(topic=topic, payload=payload)
            # client.publish(topic="ledstatus", payload=action)

    except KeyboardInterrupt as e:
        client.loop_stop()


if __name__ == "__main__":
    env = dotenv_values("../.env")
    topic = "/server/alarm"
    main(env, topic)
