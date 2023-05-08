from flask import Flask, request, render_template, jsonify, redirect, url_for
import requests
import json
import datetime

app = Flask(__name__)


@app.route("/")
def home():
    return render_template("edit.html")


@app.route("/time", methods=["GET"])
def get_time():
    response = {}
    current_time = datetime.datetime.now()
    response["hour"] = current_time.strftime("%H")
    response["minute"] = current_time.strftime("%M")
    response["date"] = current_time.strftime("%d")
    response["month"] = current_time.strftime("%m")
    response["day_of_week"] = current_time.strftime("%w")
    print(response)
    return jsonify(response)


@app.route("/weather", methods=["GET"])
def get_weather():
    WEATHER_URL = "http://api.weatherapi.com/v1/forecast.json?key=b9e41af0017f4a0cacd84627230505&q=Taipei&days=1&aqi=no&alerts=no"

    response = requests.get(WEATHER_URL).json()
    result = {}
    result["current_temp"] = response["current"]["temp_c"]
    result["max_temp"] = response["forecast"]["forecastday"][0]["day"]["maxtemp_c"]
    result["min_temp"] = response["forecast"]["forecastday"][0]["day"]["mintemp_c"]
    result["rain_probability"] = response["forecast"]["forecastday"][0]["day"][
        "daily_chance_of_rain"
    ]
    result["moisture"] = response["current"]["humidity"]
    print(result)
    return jsonify(result)


@app.route("/edit", methods=["POST"])
def edit():
    print(request.form)

    with open("data/uid.json", "r") as f:
        data = json.load(f)

    uid = request.form["uid"]
    name = request.form["name"]
    description = request.form["description"]
    icon = request.form["icon"]

    data[uid] = {"name": name, "description": description, "icon": icon}
    json.dump(data, open("data/uid.json", "w"), indent=4)

    return redirect(url_for("home"))


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=3000, debug=True)
