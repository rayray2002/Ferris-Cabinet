import os
import json
import random
import paho.mqtt.client as mqtt
import signal
from datetime import datetime
from dotenv import dotenv_values

ENV_PATH = os.path.join(os.path.dirname(__file__), "..", ".env")

env = dotenv_values(ENV_PATH)

client = mqtt.Client()


def rotate_and_scan():
    # TODO
    uid_list = ["00000000", "11111111", "22222222", "33333333", "44444444", "55555555", "66666666", "77777777", "88888888"]
    return random.sample(uid_list, 8)


def rotate_to_index(index):
    # TODO
    print(f"Rotate to {index}")


def uid_to_config(uids: list):
    with open("data/uid.json", "r") as f:
        data = json.load(f)

    config = []
    for uid in uids:
        if uid in data:
            print(f"UID: {uid}, CONFIG: {data[uid]}")
            config.append(data[uid])
        else:
            print(f"UID: {uid}, CONFIG: None")
            config.append(data["None"])

    print(f"CONFIG: {config}")
    return config


def on_message(client, obj, msg):
    print(f"TOPIC:{msg.topic}, VALUE:{msg.payload}")
    topic = msg.topic
    payload = msg.payload

    if topic == "/client/action":
        action = int(payload.decode())
        if action == 0:
            print("Action: Scan")
            uids = rotate_and_scan()
            config = uid_to_config(uids)
            config = json.dumps(config, separators=(",", ":"))
            print(f"len(config): {len(config)}")
            client.publish(topic="/server/config", payload=config)
        elif action == 9:
            print("Action: Recommend clothes")
        else:
            print(f"Action: Rotate to {action}")
            rotate_to_index(action)

    elif topic == "/client/alarm":
        payload = payload.decode()
        payload = json.loads(payload)
        print(f"Alarm: {payload}")

        hour = payload["hour"]
        minute = payload["minute"]
        print(f"Alarm fmt: {hour}:{minute}")

        now = datetime.now()
        print(f"Alarm now: {now}")
        alarm_time = now.replace(
            hour=int(hour), minute=int(minute), second=0, microsecond=0
        )
        print(f"Alarm time: {alarm_time}")
        delta = alarm_time - now
        print(f"Alarm delta: {delta}")

        signal.alarm(delta.seconds)


# {"hour":13,"minute":38,"desk":1}


def alarm_handler(signum, frame):
    print("Alarm: Timeout")
    client.publish(topic="/server/alarm", payload="1")


def main(env, topics):
    signal.signal(signal.SIGALRM, alarm_handler)

    # Establish connection to mqtt broker
    client.on_message = on_message
    client.connect(host=env["MQTT_HOST"], port=int(env["MQTT_PORT"]))

    for t in topics:
        topic = f"{t}"
        print(f"Subscribing to {topic}")
        client.subscribe(topic, 0)

    try:
        client.loop_forever()
    except KeyboardInterrupt as e:
        pass


if __name__ == "__main__":
    # get arguments
    topics = [
        "/client/action",
        "/client/alarm",
    ]
    main(env, topics)
