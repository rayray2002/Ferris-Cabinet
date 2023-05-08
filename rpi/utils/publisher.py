import time

import psutil
import paho.mqtt.client as mqtt
from dotenv import dotenv_values


def main(env):
    # Establish connection to mqtt broker
    client = mqtt.Client()
    client.connect(host=env["MQTT_HOST"], port=int(env["MQTT_PORT"]))
    client.loop_start()

    try:
        while True:
            # Fill the payload
            payload = psutil.cpu_percent()
            # Publish the message to topic
            client.publish(topic="/lamp1/temperature", payload=payload)
            client.publish(topic="/lamp1/humidity", payload=payload*2)
            client.publish(topic="/lamp1/pressure", payload=payload*10)
            client.publish(topic="/lamp1/emergency", payload=1)
            time.sleep(1)
    except KeyboardInterrupt as e:
        client.loop_stop()


if __name__ == "__main__":
    env = dotenv_values("../.env")
    main(env)