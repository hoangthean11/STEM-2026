#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>

#define DHT_PIN 4
#define DHT_TYPE DHT11
#define RELAY1_PIN 25 //đèn
#define RELAY2_PIN 26 //quạt
#define RELAY3_PIN 27 //máy hút ẩm


DHT dht(DHT_PIN, DHT_TYPE);

using namespace std;

const char* ssidList[] = {"người đẹp trai"};//{"Phong Tin 2", "DESKTOP-HTELVD4 8429"};
const char* passList[] = {"36363636"};//{"88888888", "19216819esp32"};
const int wifiCount = sizeof(ssidList) / sizeof(ssidList[0]);

bool p1 = 0, p2 = 0, p3 = 0;
float humidity = 0, temperature = 0;

WebServer server(80);

void handleRoot() {
  String html = R"rawliteral(






<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>NOVA Smart Home</title>
        <link
            rel="stylesheet"
            href="https://unpkg.com/leaflet@1.6.0/dist/leaflet.css"
            integrity="sha512-xwE/Az9zrjBIphAcBb3F6JVqxf46+CDLwfLMHloNu6KEQCAWi6HcDUbeOfBIptF7tcCzusKFjFw2yuvEpDL9wQ=="
            crossorigin=""
        />
        <style>
            :root {
                --main_hover_blue_color: rgb(26, 144, 255);
                --transparent_blue_color: rgba(0, 132, 255, 0.286);
                --transparent_new_blue_color: rgba(3, 74, 141, 0.286);
                --gadget_background_color: rgba(0, 132, 255, 0.023);
                --border_thickness: 5px;
                --border_color: rgb(0, 132, 255);
                --no_signal_error_red: rgba(200, 30, 30, 0.529);
                --text_color: rgb(255, 255, 255);
                --_green: rgb(0, 151, 35);
                --box_shadow: var(--box_shadow);
            }

            html {
                scroll-behavior: smooth;
            }

            body {
                margin: 0;
                font-family: Poppins, sans-serif;
                background-color: black;
                margin: 0;
                height: 500vh;
                color: var(--text_color);
                background-color: var(--text_color);
                overflow: hidden;
            }

            /*.main *{
                transition: all 1s;
            }
            */

            @media (max-width: 700px) {
                #temp_chart,
                #humidity_chart,
                #aq_chart,
                #page2,
                #page3 {
                    display: none;
                }
                .main {
                    flex-direction: column;
                    align-items: center;
                    width: 100%;
                }

                .left {
                    margin: 0px;
                    width: 90%;
                    font-family: Poppins, sans-serif;
                    color: var(--text_color);
                    display: flex;
                    justify-content: space-between;
                    flex-direction: column;
                }

                .right {
                    margin: 0px;
                    width: 90%;
                    font-family: Poppins, sans-serif;
                    color: var(--text_color);
                    display: flex;
                    justify-content: space-between;
                    flex-direction: column;
                    align-items: center;
                }

                .top {
                    flex-direction: column;
                    height: auto;
                }

                .thaq_gadget {
                    margin-bottom: 10px;
                }

                .controls {
                    flex-direction: column;
                    align-items: center;
                }

                .control {
                    width: 80%;
                }

                .real_weather div {
                    width: 100%;
                    flex-direction: column;
                    align-items: center;
                    gap: 10px;
                }

                .real_weather {
                    margin-bottom: 20px;
                }

                .map {
                    margin-bottom: 20px;
                }

                .air_quality {
                    width: 250px;
                    height: 250px;
                    display: flex;
                    justify-content: space-around;
                    align-items: center;
                    border-radius: 100%;
                    background: var(--good_green);
                    transition-duration: 1s;
                    flex-direction: column;
                    padding: 20px;
                }

                .air_quality h1,
                .air_quality h5,
                .air_quality h3 {
                    font-size: 15px;
                    margin: 0px;
                }
            }

            @media (max-width: 1200px) and (min-width: 700px) {
                .main {
                    flex-direction: column;
                    align-items: center;
                    width: 100%;
                }

                .left {
                    margin: 0px;
                    width: 90%;
                    font-family: Poppins, sans-serif;
                    color: var(--text_color);
                    display: flex;
                    justify-content: space-between;
                    flex-direction: column;
                }

                .right {
                    margin: 0px;
                    width: 90%;
                    font-family: Poppins, sans-serif;
                    color: var(--text_color);
                    display: flex;
                    justify-content: space-between;
                    flex-direction: column;
                    align-items: center;
                }

                .top {
                    flex-direction: column;
                    height: auto;
                }

                .thaq_gadget {
                    margin-bottom: 10px;
                }

                .controls {
                    flex-direction: column;
                    align-items: center;
                }

                .control {
                    width: 80%;
                }

                .real_weather div {
                    width: 100%;
                    flex-direction: column;
                    align-items: center;
                    gap: 10px;
                }

                .real_weather {
                    margin-bottom: 20px;
                }

                .map {
                    margin-bottom: 20px;
                }

                .air_quality {
                    width: 250px;
                    height: 250px;
                    display: flex;
                    justify-content: space-around;
                    align-items: center;
                    border-radius: 100%;
                    background: var(--good_green);
                    transition-duration: 1s;
                    flex-direction: column;
                    padding: 20px;
                }

                .air_quality h1,
                .air_quality h5,
                .air_quality h3 {
                    font-size: 15px;
                    margin: 0px;
                }
            }

            .main {
                padding-top: 50px;
                display: flex;
                justify-content: space-around;
                height: calc(100vh - 50px);
                overflow-y: auto;
            }

            @media (min-width: 1200px) {
                .left {
                    margin: 0px;
                    width: 45%;
                    font-family: Poppins, sans-serif;
                    color: var(--text_color);
                    display: flex;
                    justify-content: space-between;
                    flex-direction: column;
                }

                .right {
                    margin: 0px;
                    width: 45%;
                    font-family: Poppins, sans-serif;
                    color: var(--text_color);
                    display: flex;
                    justify-content: space-between;
                    flex-direction: column;
                    align-items: center;
                }

                .real_weather div {
                    width: 50%;
                    display: flex;
                    flex-direction: row;
                    justify-content: space-between;
                    align-items: left;
                    margin-left: 20px;
                }

                .air_quality {
                    width: 400px;
                    height: 400px;
                }
            }

            .top {
                display: flex;
                justify-content: space-around;
                align-items: center;
                width: 100%;
            }

            .thaq_gadget {
                width: 150px;
                height: 150px;
                display: flex;
                flex-direction: column;
                justify-content: center;
                align-items: center;
                border-radius: 10px;
                background: var(--transparent_blue_color);
                transition-duration: 1s;
                border: var(--border_thickness) solid var(--border_color);
            }

            .thaq_gadget:hover {
                background: var(--main_hover_blue_color);
                margin-top: -5px;
                margin-left: -5px;
                box-shadow: 10px 10px 0px var(--box_shadow);
                transition-duration: 0.25s;
            }

            .real_weather {
                width: 100%;
                min-height: 150px;
                display: flex;
                justify-content: left;
                align-items: center;
                border-radius: 10px;
                background: var(--transparent_new_blue_color);
                transition-duration: 1s;
                flex-wrap: wrap;
                border: var(--border_thickness) solid var(--border_color);
            }

            .real_weather:hover {
                background: var(--main_hover_blue_color);
                margin-top: -5px;
                margin-left: -5px;
                box-shadow: 10px 10px 0px var(--box_shadow);
                transition-duration: 0.25s;
            }

            .real_weather img {
                width: 100px;
                height: 100px;
                margin: 25px;
            }

            .real_weather div div {
                width: 50%;
                display: flex;
                flex-direction: column;
                justify-content: center;
                align-items: left;
            }

            .map {
                width: 95%;
                height: 400px;
                display: flex;
                justify-content: space-around;
                align-items: left;
                border-radius: 10px;
                background: var(--border_color);
                transition-duration: 1s;
                flex-direction: column;
            }

            .map:hover {
                margin-top: -15px;
                margin-left: -5px;
                box-shadow: 10px 10px 0px var(--box_shadow);
                transition-duration: 0.25s;
            }

            .map #your_map {
                width: 100%;
                height: 350px;
                border-radius: 10px;
                z-index: 1;
            }

            .house_condition {
                text-align: center;
                width: 100%;
                min-height: 300px;
                display: flex;
                justify-content: space-around;
                flex-direction: column;
                border-radius: 10px;
                background: var(--gadget_background_color);
                transition-duration: 1s;
                border: var(--border_thickness) solid var(--border_color);
                flex-wrap: wrap;
                margin: 0px 0px 20px 0px;
            }

            .BacNinh_condition {
                text-align: center;
                align-items: center;
                width: 100%;
                min-height: 800px;
                display: flex;
                justify-content: space-around;
                flex-direction: column;
                flex-wrap: wrap;
                border-radius: 10px;
                background: var(--gadget_background_color);
                transition-duration: 1s;
                border: var(--border_thickness) solid var(--border_color);
            }

            .air_rating {
                text-align: center;
                width: 100%;
                height: 500px;
                display: flex;
                flex-direction: column;
                justify-content: space-around;
                align-items: center;
                border-radius: 10px;
                background: var(--transparent_new_blue_color);
                border: var(--border_thickness) solid var(--border_color);
                transition-duration: 1s;
                margin-bottom: 20px;
            }

            .info {
                text-align: center;
                width: 100%;
                min-height: 150px;
                display: flex;
                justify-content: space-around;
                align-items: center;
                border-radius: 10px;
                background: var(--transparent_new_blue_color);
                border: var(--border_thickness) solid var(--border_color);
                transition-duration: 1s;
                flex-direction: row;
                margin-bottom: 20px;
            }

            .info:hover {
                background: var(--main_hover_blue_color);
                margin-top: -5px;
                margin-left: -5px;
                box-shadow: 10px 10px 0px var(--box_shadow);
                transition-duration: 0.25s;
            }

            .time {
                display: flex;
                width: 45%;
                flex-direction: column;
            }

            .info img {
                height: 75px;
            }

            .air_quality {
                display: flex;
                justify-content: space-around;
                align-items: center;
                border-radius: 100%;
                background: var(--good_green);
                transition-duration: 1s;
                flex-direction: column;
                padding: 20px;
            }

            .air_quality h1 {
                font-size: 70px;
                margin: 0px;
            }

            .aq {
                display: flex;
                justify-content: space-around;
                flex-direction: row;
            }

            .aq div {
                margin: 20px;
            }

            .control_panel {
                text-align: center;
                width: 100%;
                display: flex;
                justify-content: space-around;
                flex-direction: column;
                align-items: center;
                border-radius: 10px;
                background: var(--transparent_new_blue_color);
                border: var(--border_thickness) solid var(--border_color);
                transition-duration: 1s;
                min-height: 500px;
            }

            .controls {
                width: 100%;
                margin-top: 20px;
                display: flex;
                justify-content: space-around;
                flex-direction: row;
                flex-wrap: wrap;
            }

            .control {
                width: 150px;
                height: 150px;
                display: flex;
                flex-direction: column;
                justify-content: center;
                align-items: center;
                border-radius: 10px;
                background: var(--transparent_blue_color);
                transition-duration: 1s;
                flex-wrap: wrap;
                border: var(--border_thickness) solid var(--border_color);
                margin: 10px;
            }

            .control:hover {
                background: var(--main_hover_blue_color);
                box-shadow: 10px 10px 0px var(--box_shadow);
                transition-duration: 0.25s;
            }

            .switch {
                position: relative;
                display: inline-block;
                width: 60px;
                height: 34px;
            }

            /* Hide default HTML checkbox */
            .switch input {
                opacity: 0;
                width: 0;
                height: 0;
            }

            /* The slider */
            .slider {
                position: absolute;
                cursor: pointer;
                top: 0;
                left: 0;
                right: 0;
                bottom: 0;
                background-color: #ee5b5b;
                -webkit-transition: 0.4s;
                transition: 0.4s;
            }

            .slider:before {
                position: absolute;
                content: "";
                height: 26px;
                width: 26px;
                left: 4px;
                bottom: 4px;
                background-color: var(--text_color);
                -webkit-transition: 0.4s;
                transition: 0.4s;
            }

            input:checked + .slider {
                background-color: #5cd641;
            }

            input:focus + .slider {
                box-shadow: 0 0 1px #2196f3;
            }

            input:checked + .slider:before {
                -webkit-transform: translateX(26px);
                -ms-transform: translateX(26px);
                transform: translateX(26px);
            }

            /* Rounded sliders */
            .slider.round {
                border-radius: 34px;
            }

            .slider.round:before {
                border-radius: 50%;
            }

            .page_2,
            .page_3,
            .page_4 {
                margin-top: 200px;
                display: flex;
                align-items: center;
                flex-direction: column;
                width: 100%;
                height: 100vh;
                overflow-y: auto;
            }

            .page_2 h1,
            .page_3 h1 {
                padding: 20px;
            }

            .controls_container {
                width: 80%;
                display: flex;
                justify-content: space-around;
                flex-wrap: wrap;
            }

            .chart-container {
                display: flex;
                background: var(--transparent_new_blue_color);
                flex-direction: row;
                flex-wrap: wrap;
                justify-content: space-around;
                width: 90%;
                padding: 10px;
                border: var(--border_thickness) solid var(--border_color);
                border-radius: 10px;
            }

            .chart-box {
                width: 50%;
                min-width: 600px;
                height: 400px;
            }

            .chart-box svg {
                height: 100%;
            }

            .background_blur {
                position: absolute;
                top: 0;
                left: 0;
                width: 100%;
                height: 600vh;
                background-image: url("https://github.com/hoangthean11/esp32_smart_home/blob/main/background2.jpg?raw=true");
                background-repeat: repeat-y;
                background-position: center top;
                background-size: 100%;

                z-index: -1;
                transition: 3s;
            }

            #nav {
                display: flex;
                flex-direction: column;

                align-items: center;
                position: fixed;
                top: 0;
                left: -295px;
                width: 300px;
                height: 100vh;
                color: none;
                background: none;
                transition: 0.5s;
                z-index: 5;
            }

            #nav img {
                width: 75%;
                text-align: center;
            }

            #nav h2 {
                width: 300px;
                height: 200px;
                text-align: center;
            }

            #nav a {
                width: 300px;
                height: 50px;
                text-decoration: none;
                color: var(--text_color);
                align-items: center;
            }

            #nav a:hover {
                width: 300px;
                height: 50px;
                text-decoration: none;
                color: var(--text_color);
                align-items: center;
                background-color: rgb(0, 46, 86);
            }

            #nav:hover {
                left: 0px;
                color: var(--text_color);
                background-color: rgb(36, 36, 36);
            }
        </style>
        <link rel="preconnect" href="https://fonts.googleapis.com" />
        <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin />
        <link
            href="https://fonts.googleapis.com/css2?family=Oswald:wght@200..700&family=Poppins:ital,wght@0,100;0,200;0,300;0,400;0,500;0,600;0,700;0,800;0,900;1,100;1,200;1,300;1,400;1,500;1,600;1,700;1,800;1,900&family=Roboto+Condensed:ital,wght@0,100..900;1,100..900&display=swap"
            rel="stylesheet"
        />

        <link
            rel="stylesheet"
            href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined"
        />
    </head>
    <body>
        <div
            id="loading_screen"
            style="
                position: fixed;
                display: flex;
                align-items: center;
                z-index: 15;
                width: 100%;
                height: 100vh;
                flex-direction: column;
                background: black;
                transition: 2s;
            "
        >
            <img
                src="https://github.com/hoangthean11/esp32_smart_home/blob/main/bea83775357853.5c4a1808c8a7b.gif?raw=true"
                alt=""
                style="margin: auto 0"
            />
        </div>
        <div id="page1" style="position: absolute; top: -10px"></div>
        <div class="background_blur"></div>
        <div class="main pages">
            <div class="left">
                <div class="house_condition">
                    <h2>House condition</h2>
                    <div class="top">
                        <a
                            href="#page2"
                            style="
                                color: var(--text_color);
                                text-decoration: none;
                            "
                        >
                            <div class="thaq_gadget temp_sensor_1">
                                <p class="title">
                                    <span
                                        class="material-symbols-outlined"
                                        style="
                                            font-size: 30px;
                                            position: relative;
                                            bottom: -8px;
                                        "
                                    >
                                        thermostat </span
                                    >Temperature
                                </p>
                                <h1 id="temp_sensor_1">---°C</h1>
                            </div>
                        </a>
                        <a
                            href="#page2"
                            style="
                                color: var(--text_color);
                                text-decoration: none;
                            "
                        >
                            <div class="thaq_gadget humid_sensor_1">
                                <p class="title">
                                    <span
                                        class="material-symbols-outlined"
                                        style="
                                            font-size: 30px;
                                            position: relative;
                                            bottom: -8px;
                                        "
                                    >
                                        humidity_percentage </span
                                    >Humidity
                                </p>
                                <h1 id="humid_sensor_1">---%</h1>
                            </div>
                        </a>
                        <a
                            href="#page3"
                            style="
                                color: var(--text_color);
                                text-decoration: none;
                            "
                        >
                            <div class="thaq_gadget airq_sensor_1">
                                <p class="title">
                                    <span
                                        class="material-symbols-outlined"
                                        style="
                                            font-size: 30px;
                                            position: relative;
                                            bottom: -8px;
                                        "
                                    >
                                        detector_co </span
                                    >Air Quality
                                </p>
                                <h1 id="airq_sensor_1">---ppm</h1>
                            </div>
                        </a>
                    </div>
                </div>
                <div class="BacNinh_condition">
                    <h2>Bac Ninh condition</h2>
                    <a
                        href="https://google.com/search?q=bac ninh weather"
                        target="_blank"
                        style="
                            color: var(--text_color);
                            text-decoration: none;
                            width: 95%;
                        "
                    >
                        <div class="real_weather">
                            <img src="#" alt="" id="condition_icon" />
                            <div>
                                <div>
                                    <h1 id="temp">---°C</h1>
                                    <h1 id="humid">---%</h1>
                                </div>
                                <div>
                                    <h3 id="condition">---</h3>
                                    <h4 id="location">---</h4>
                                </div>
                            </div>
                        </div>
                    </a>
                    <div class="map">
                        <a
                            href="https://maps.google.com/maps?ll=21.191977,106.074229&z=18&t=h&hl=en&gl=US&mapclient=embed&q=21%C2%B011%2731.3%22N%20106%C2%B004%2729.3%22E%2021.192028%2C%20106.074806@21.1920278,106.0748056"
                            style="
                                color: var(--text_color);
                                text-decoration: none;
                            "
                        >
                            <h3>Your current position</h3></a
                        >
                        <!-- <iframe
                                src="https://www.google.com/maps/embed?pb=!1m17!1m12!1m3!1d1729.2204111624956!2d106.07401203938154!3d21.192016998173795!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m2!1m1!2zMjHCsDExJzMxLjMiTiAxMDbCsDA0JzI5LjMiRQ!5e1!3m2!1sen!2sus!4v1747200455001!5m2!1sen!2sus"
                                width="100%"
                                height="350px"
                                style="border: 0"
                                allowfullscreen
                                loading="lazy"
                                referrerpolicy="no-referrer-when-downgrade"
                            ></iframe> -->
                        <div id="your_map"></div>
                    </div>
                </div>
            </div>
            <div class="right">
                <div class="info">
                    <div class="time">
                        <h1 id="time">11:22</h1>
                        <h3 id="date">5/13/2025</h3>
                    </div>
                    <div class="esp_info">
                        <img
                            src="https://github.com/hoangthean11/esp32_smart_home/blob/main/esp32_icon.png?raw=true"
                            alt=""
                        />
                        <h3>ESP32 WROOM 32</h3>
                    </div>
                </div>
                <div class="air_rating">
                    <h2>Air Quality</h2>
                    <div class="air_quality">
                        <h2 id="aq_status" style="font-size:40px">Good</h2>
                        <div class="aq">
                            <div class="air_quality_1">
                                <h5>PM2.5</h5>
                                <h3 id="pm25">---µg/m³</h3>
                            </div>
                            <div class="air_quality_2">
                                <h5>PM10</h5>
                                <h3 id="pm10">---µg/m³</h3>
                            </div>
                            <div class="air_quality_3">
                                <h5>CO</h5>
                                <h3 id="co">---ppm</h3>
                            </div>
                        </div>
                    </div>
                </div>
                <a
                    class="control_panel"
                    href="#page4"
                    style="color: var(--text_color); text-decoration: none"
                >
                    <h2>Control Panel</h2>
                    <div class="controls">
                        <div class="control" id="light_1_control">
                            <p class="title">Light 1</p>
                            <label class="switch">
                                <input type="checkbox" onchange="toggle(this, 'light1')"/>
                                <span
                                    class="slider round"
                                ></span>
                            </label>
                        </div>
                        <div class="control" id="fan_1_control">
                            <p class="title">Fan</p>
                            <label class="switch">
                                <input type="checkbox" onchange="toggle(this, 'fan')"/>
                                <span
                                    class="slider round"
                                ></span>
                            </label>
                        </div>
                        <div class="control" id="dehydrator_1_control">
                            <p class="title">Dehydrator</p>
                            <label class="switch">
                                <input type="checkbox" onchange="toggle(this, 'dehydrator')"/>
                                <span
                                    class="slider round"
                                ></span>
                            </label>
                        </div>
                        <div
                            class="control"
                            id="camera_2_control"
                            style="
                                background: rgba(54, 54, 54, 0.712);
                                z-index: 2;
                            "
                        >
                            <p class="title">
                                Camera 1 <br />
                                Unavailable
                            </p>
                            <div onclick="">
                                <label class="switch" style="z-index: -1">
                                    <input
                                        type="checkbox"
                                        style="z-index: -1"
                                    />
                                    <span
                                        class="slider round"
                                        style="z-index: -1"
                                    ></span>
                                </label>
                            </div>
                        </div>
                        <div class="control" id="dht11_control">
                            <p class="title">DHT sensor</p>
                            <label class="switch">
                                <input type="checkbox" checked onchange="toggle(this, "DHT11")" />
                                <span
                                    class="slider round"
                                ></span>
                            </label>
                        </div>
                        <div class="control">
                            <p class="title">...</p>
                            <form action=""></form>
                        </div>
                    </div>
                </a>
            </div>
        </div>
        <div class="page_2 pages">
            <h1 id="page2">Temperature and humidity</h1>
            <div class="chart-container">
                <div id="temp_chart" class="chart-box"></div>
                <div id="humidity_chart" class="chart-box"></div>
            </div>
        </div>
        <div class="page_3 pages">
            <h1 id="page3">Air quality</h1>
            <div class="chart-container">
                <div id="aq_chart" class="chart-box"></div>
            </div>
        </div>
        <div class="page_4 pages" id="page4">
            <h1>Sensors and Controls</h1>
            <div class="controls_container">
                <div class="control" id="light_1_control2">
                    <p class="title">Light 1</p>
                    <label class="switch">
                        <input type="checkbox" onchange="toggle(this, 'light1')"/>
                        <span
                            class="slider round"
                        ></span>
                    </label>
                </div>
                <div class="control" id="fan_1_control2">
                    <p class="title">Fan</p>
                    <label class="switch">
                        <input type="checkbox" onchange="toggle(this, 'fan')"/>
                        <span
                            class="slider round"
                        ></span>
                    </label>
                </div>
                <div class="control" id="dehydrator_1_control2">
                    <p class="title">Dehydrator</p>
                    <label class="switch">
                        <input type="checkbox" onchange="toggle(this, 'dehydrator')"/>
                        <span
                            class="slider round"
                        ></span>
                    </label>
                </div>
                <div
                    class="control"
                    id="camera_2_control2"
                    style="background: rgba(54, 54, 54, 0.712); z-index: 2"
                >
                    <p class="title">
                        Camera 1 <br />
                        Unavailable
                    </p>
                    <div onclick="">
                        <label class="switch" style="z-index: -1">
                            <input type="checkbox" style="z-index: -1" />
                            <span
                                class="slider round"
                                style="z-index: -1"
                            ></span>
                        </label>
                    </div>
                </div>
                <div class="control" id="dht11_control2">
                    <p class="title">DHT sensor</p>
                    <label class="switch">
                        <input type="checkbox" checked onclick="toggle(this, "DHT11")"/>
                        <span
                            class="slider round"
                            onclick="activate('DHT11')"
                        ></span>
                    </label>
                </div>
                <div class="control">
                    <p class="title">...</p>
                    <form action=""></form>
                </div>
            </div>
        </div>
        <a
            href="#page1"
            style="
                position: fixed;
                bottom: 20px;
                right: 20px;
                text-decoration: none;
                color: var(--text_color);
                width: 50px;
                height: 50px;
                border-radius: 25px;
                background: var(--border_color);
            "
            ><span
                class="material-symbols-outlined"
                style="position: relative; top: 5px; left: 7px; font-size: 40px"
            >
                arrow_upward
            </span></a
        >
        <div id="nav" style="z-index: 5; color: white">
            <img
                src="https://github.com/hoangthean11/esp32_smart_home/blob/88b90e43e09e684f026984ae161df746524d1057/pngtree-circular-world-flag-vietnam-icon-png-image_4830539.png?raw=true"
                alt=""
            />
            <h3>Nova Smart Home</h3>
            <a href="#page1" style="color: white">Main page</a>
            <a href="#page2" style="color: white">Temperature and Humidity</a>
            <a href="#page3" style="color: white">Air quality</a>
            <a href="#page4" style="color: white">Sensors and Controls</a>
            <button
                onclick="change_background()"
                style="
                    color: white;
                    width: 70%;
                    height: 40px;
                    background: var(--main_hover_blue_color);
                    border-radius: 10px;
                    border: none;
                "
            >
                Change color
            </button>
        </div>
    </body>
    <script
        type="text/javascript"
        src="https://www.gstatic.com/charts/loader.js"
    ></script>
    <script src="https://unpkg.com/leaflet/dist/leaflet.js"></script>
    <script type="text/javascript">
        google.charts.load("current", { packages: ["corechart"] });
        google.charts.setOnLoadCallback(drawCharts);

        const jsonData = `[
        {"hour": 1, "temp": 15, "humidity": 90, "aq": 103},
        {"hour": 2, "temp": 14, "humidity": 91, "aq": 100},
        {"hour": 3, "temp": 13, "humidity": 92, "aq": 200},
        {"hour": 4, "temp": 13, "humidity": 91, "aq": 302},
        {"hour": 5, "temp": 12, "humidity": 90, "aq": 100},
        {"hour": 6, "temp": 13, "humidity": 88, "aq": 500},
        {"hour": 7, "temp": 14, "humidity": 85, "aq": 100},
        {"hour": 8, "temp": 16, "humidity": 80, "aq": 300},
        {"hour": 9, "temp": 18, "humidity": 75, "aq": 190},
        {"hour": 10, "temp": 20, "humidity": 70, "aq": 140},
        {"hour": 11, "temp": 21, "humidity": 68, "aq": 100},
        {"hour": 12, "temp": 22, "humidity": 65, "aq": 100},
        {"hour": 13, "temp": 23, "humidity": 63, "aq": 100},
        {"hour": 14, "temp": 24, "humidity": 60, "aq": 120},
        {"hour": 15, "temp": 25, "humidity": 58, "aq": 190},
        {"hour": 16, "temp": 25, "humidity": 57, "aq": 300},
        {"hour": 17, "temp": 24, "humidity": 59, "aq": 500},
        {"hour": 18, "temp": 23, "humidity": 61, "aq": 600},
        {"hour": 19, "temp": 22, "humidity": 65, "aq": 700},
        {"hour": 20, "temp": 20, "humidity": 70, "aq": 100},
        {"hour": 21, "temp": 18, "humidity": 75, "aq": 1000},
        {"hour": 22, "temp": 17, "humidity": 80, "aq": 100},
        {"hour": 23, "temp": 16, "humidity": 85, "aq": 100},
        {"hour": 24, "temp": 15, "humidity": 88, "aq": 100}
        ]`;
        let current_theme = 1;

        function change_background() {
            let root = document.querySelector(":root");
            let background_blur =
                document.getElementsByClassName("background_blur")[0];
            let body = document.getElementsByTagName("BODY")[0];
            current_theme += 1;
            console.log("change");

            if (current_theme >= 4) {
                current_theme = 1;
            }

            if (current_theme === 1) {
                background_blur.style.display = "block";
                body.style.background = "black";
                root.style.setProperty("--text_color", "white");
            } else if (current_theme === 2) {
                background_blur.style.display = "none";
                body.style.background = "black";
                root.style.setProperty("--text_color", "white");
            } else if (current_theme === 3) {
                background_blur.style.display = "none";
                body.style.background = "white";
                root.style.setProperty("--text_color", "black");
            }
        }

        function drawCharts() {
            const raw = JSON.parse(jsonData);

            const tempData = [["Hour", "Temperature"]];
            const humidityData = [["Hour", "Humidity"]];
            const air_qualityData = [["Hour", "Quality"]];
            raw.forEach((entry) => {
                const label = `${entry.hour}`;
                tempData.push([label, entry.temp]);
                humidityData.push([label, entry.humidity]);
                air_qualityData.push([label, entry.aq]);
            });

            const tempOptions = {
                title: "Temperature Over 24 Hours",
                legendTextStyle: { color: "#f0f0f0" },
                titleTextStyle: { color: "#f0f0f0" },
                hAxis: {
                    title: "Hour",
                    gridlines: { count: 24, color: "#f0f0f0" },
                    textStyle: { color: "#f0f0f0" },
                    titleTextStyle: { color: "#f0f0f0" },
                },
                vAxis: {
                    title: "Temperature (°C)",
                    gridlines: { color: "#f0f0f0" },
                    textStyle: { color: "#f0f0f0" },
                    titleTextStyle: { color: "#f0f0f0" },
                },
                legend: "none",
                colors: ["#4285F4"],
                areaOpacity: 0.3,
                chartArea: { width: "85%", height: "70%" },
                backgroundColor: { fill: "none" },
            };

            const humidityOptions = {
                title: "Humidity Over 24 Hours",
                legendTextStyle: { color: "#f0f0f0" },
                titleTextStyle: { color: "#f0f0f0" },
                hAxis: {
                    title: "Hour",
                    gridlines: { count: 24, color: "#f0f0f0" },
                    textStyle: { color: "#f0f0f0" },
                    titleTextStyle: { color: "#f0f0f0" },
                },
                vAxis: {
                    title: "Humidity (%)",
                    gridlines: { color: "#f0f0f0" },
                    textStyle: { color: "#f0f0f0" },
                    titleTextStyle: { color: "#f0f0f0" },
                },
                legend: "none",
                colors: ["#34A853"],
                areaOpacity: 0.3,
                chartArea: { width: "85%", height: "70%" },
                backgroundColor: { fill: "none" },
            };

            const air_qualityOption = {
                title: "Air qualtity Over 24 Hours",
                legendTextStyle: { color: "#f0f0f0" },
                titleTextStyle: { color: "#f0f0f0" },
                hAxis: {
                    title: "Hour",
                    gridlines: { count: 24, color: "#f0f0f0" },
                    textStyle: { color: "#f0f0f0" },
                    titleTextStyle: { color: "#f0f0f0" },
                },
                vAxis: {
                    title: "CO (ppm)",
                    gridlines: { color: "#f0f0f0" },
                    textStyle: { color: "#f0f0f0" },
                    titleTextStyle: { color: "#f0f0f0" },
                },
                legend: "none",
                colors: ["#863CF2"],
                areaOpacity: 0.3,
                chartArea: { width: "85%", height: "70%" },
                backgroundColor: { fill: "none" },
            };

            const tempChart = new google.visualization.AreaChart(
                document.getElementById("temp_chart")
            );
            const humidityChart = new google.visualization.AreaChart(
                document.getElementById("humidity_chart")
            );
            const air_qualityChart = new google.visualization.AreaChart(
                document.getElementById("aq_chart")
            );

            tempChart.draw(
                google.visualization.arrayToDataTable(tempData),
                tempOptions
            );
            humidityChart.draw(
                google.visualization.arrayToDataTable(humidityData),
                humidityOptions
            );
            air_qualityChart.draw(
                google.visualization.arrayToDataTable(air_qualityData),
                air_qualityOption
            );
        }
        function update_map(pos) {
            console.log(pos);
            var map = L.map("your_map").setView([0, 0], 2);
            
            L.tileLayer("https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png", {
                maxZoom: 19,
            }).addTo(map);
            map.setView(pos, 15);
            L.marker(pos).addTo(map).openPopup();
        }

        function clear_loading() {
            loading_screen = document.getElementById("loading_screen");
            loading_screen.style.opacity = "0";
            setTimeout(() => {
                loading_screen.style.display = "none";
            }, 2000);
        }

        var state = {
            light1: false,
            fan: false,
            dehydrator: false,
            camera1: false,
            DHT11: true,
        };

        function setLight(light, value) {
            fetch("/lights", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify({
                    light: light,
                    value: value
                })
            });
        }

        function activate(x) {
            state[x] = 1;
            console.log(`${x}: ${state[x]}`);
            setLight(x, 1);
        }

        function deactivate(x) {
            state[x] = 0;
            console.log(`${x}: ${state[x]}`);
            setLight(x, 0);
        }

        function toggle(cb, light) {
            const value = cb.checked ? 1 : 0;
            if (value) activate(light);
            else deactivate(light);
        }



        const apiKey = "c26bb482498344d4b25120857250305";

        async function getWeatherData(location) {
            let response = await fetch(
                `https://api.weatherapi.com/v1/current.json?key=${apiKey}&q=${`${location[0]},${location[1]}`}&aqi=yes`
            );
            let data = await response.json();
            return data;
        }
        
        function updateAQStatus(epaIndex) {
            const statusEl = document.getElementById("aq_status");
            const circle = document.querySelector(".air_quality");

            const map = {
                1: { text: "Good", color: "rgb(0,151,35)" },
                2: { text: "Moderate", color: "#f1c40f" },
                3: { text: "Unhealthy (SG)", color: "#e67e22" },
                4: { text: "Unhealthy", color: "#e74c3c" },
                5: { text: "Very Unhealthy", color: "#8e44ad" },
                6: { text: "Hazardous", color: "#2c3e50" }
            };

            const level = map[epaIndex] || map[1];

            statusEl.innerText = level.text;
            circle.style.background = level.color;
        }


        function get_geolocation(callback) {
            if (navigator.geolocation) {
                navigator.geolocation.getCurrentPosition(
                    function (position) {
                        const pos = [
                            position.coords.latitude,
                            position.coords.longitude,
                        ];
                        console.log("Geolocation success:", pos);
                        return callback([21.1925554393877, 106.07505468684786]); // Call the update_map function when position is ready
                    },
                    function (error) {
                        console.log("Geolocation failed: " + error.message);
                        return callback([21.1925554393877, 106.07505468684786]);
                    }
                );
            } else {
                alert("Geolocation is not supported by your browser.");
            }
        }

        function update(
            icon,
            temp,
            condition,
            location,
            felling,
            humidity,
            airq
        ) {
            let main = document.querySelector("#main");
            let pm25 = document.querySelector("#pm25");
            let pm10 = document.querySelector("#pm10");
            let co = document.querySelector("#co");
            pm10.innerText = `${airq.pm10} µg/m³`;
            pm25.innerText = `${airq.pm2_5} µg/m³`;
            co.innerText = `${airq.co} ppm`;
            document.getElementById("temp").innerText = `${temp}°C`;
            document.getElementById("humid").innerText = `${humidity}%`;
            document.getElementById("condition").innerText = condition;
            document.getElementById("location").innerText = location;
            document.getElementById("condition_icon").src = icon;
            document.getElementById(
                        "airq_sensor_1"
                    ).innerText = `${airq.co} ppm`;
        }

        function fetchSensors() {
            fetch("/sensors")
                .then((res) => {
                    if (!res.ok) {
                        throw new Error("Network response was not ok");
                    }
                    return res.json();
                })
                .then((data) => {
                    console.log(`${data.temperature}°C`);
                    document.getElementById(
                        "temp_sensor_1"
                    ).innerText = `${data.temperature}°C`;
                    document.getElementById(
                        "humid_sensor_1"
                    ).innerText = `${data.humidity}%`;
                    
                })
                .catch((error) => {
                    console.log(error);
                    return NaN;
                });
        }

        function fetch_lights() {
          fetch("/lights")
                .then((res) => {
                    if (!res.ok) {
                        throw new Error("Network response was not ok");
                    }
                    return res.json();
                })
                .then((data) => {
                    let l1 = data.p1;
                    let l2 = data.p2;
                    let l3 = data.p3;

                    if (l1) console.log("Light 1 turn on");

                    if (l2) console.log("Light 2 turn on");
                    
                    if (l3) console.log("Light 3 turn on");

                    document.querySelector('#light_1_control input').checked = data.p1;
                    document.querySelector('#fan_1_control input').checked = data.p2;
                    document.querySelector('#dehydrator_1_control input').checked = data.p3;
                    
                    document.querySelector('#light_1_control2 input').checked = data.p1;
                    document.querySelector('#fan_1_control2 input').checked = data.p2;
                    document.querySelector('#dehydrator_1_control2 input').checked = data.p3;

                    state["light1"] = data.p1;
                    state["fan"] = data.p2;
                    state["dehydrator"] = data.p3;
                    
                    console.log(state);
                })
                .catch((error) => {
                    console.log(error);
                    return NaN;
                });
        }

        let temp1 = document.getElementById("temp_sensor_1");
        let humid1 = document.getElementById("humid_sensor_1");
        let airq1 = document.getElementById("airq_sensor_1");
        
        function setup() {
            get_geolocation(async function (pos) {
                try {
                    const data = await getWeatherData(pos);
                    let temp = data.current.temp_c;
                    let condition = data.current.condition.text;
                    let felling = data.current.feelslike_c;
                    let location = data.location.name;
                    let humidity = data.current.humidity;
                    let icon = "https:" + data.current.condition.icon;
                    let airq = data.current.air_quality;

                    update(
                        icon,
                        temp,
                        condition,
                        location,
                        felling,
                        humidity,
                        airq
                    );
                    updateAQStatus(airq["us-epa-index"]);
                } catch (error) {
                    console.log("Weather API error:", error);
                }
            });
            get_geolocation(update_map);
        }

        function main() {
            //Đỏ nếu chưa có dữ liệu
            if (temp1.innerText == "---°C") {
                temp1.parentElement.style.background =
                    "var(--no_signal_error_red)";
            } else {
                temp1.parentElement.style.background =
                    "var(--transparent_blue_color)";
            }
            if (humid1.innerText == "---%") {
                humid1.parentElement.style.background =
                    "var(--no_signal_error_red)";
            } else {
                humid1.parentElement.style.background =
                    "var(--transparent_blue_color)";
            }
            if (airq1.innerText == "---ppm") {
                airq1.parentElement.style.background =
                    "var(--no_signal_error_red)";
            } else {
                airq1.parentElement.style.background =
                    "var(--transparent_blue_color)";
            }
            document.getElementById("time").innerText =
                new Date().toLocaleTimeString();
            document.getElementById("date").innerText =
                new Date().toLocaleDateString();

            fetchSensors();
            fetch_lights();
            console.log("finish");
        }

        setup();
        main();
        //update_controls();
        //clear_loading();
        setTimeout(() => {
            clear_loading();
        }, 4750);
        
        setInterval(setup, 5000);
        setInterval(main, 1000);
    </script>
</html>









  )rawliteral";

  server.send(200, "text/html", html);
}

void handle_hardware() {
    humidity = dht.readHumidity();

    temperature = dht.readTemperature();

    String json = "{";
    json += "\"temperature\":" + String(temperature) + ",";
    json += "\"humidity\":" + String(humidity) + ",";
    json += "\"airQuality\":" + String(humidity);
    json += "}";
    
    server.send(200, "application/json", json);
}

void handle_lights() {

  String json = "{";
  json += "\"p1\":" + String(p1 ? "true" : "false") + ",";
  json += "\"p2\":" + String(p2 ? "true" : "false") + ",";
  json += "\"p3\":" + String(p3 ? "true" : "false");
  json += "}";
  server.send(200, "application/json", json);
}

void handle_logic() {
    if (temperature > 30) {
        p2 = 1;
        p1 = 0;
    }
    else if (temperature < 15) {
        p1 = 1;
        p2 = 0;
    }

    if (humidity > 90) {
        p3 = 1;
    }
    else if (humidity <= 60) {
        p3 = 0;
    }

    digitalWrite(RELAY1_PIN, p1);
    digitalWrite(RELAY2_PIN, p2);
    digitalWrite(RELAY3_PIN, p3);
}

void handle_set_lights_from_web() {
    if (!server.hasArg("plain")) {
        server.send(400, "application/json", "{\"error\":\"No body\"}");
        return;
    }

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, server.arg("plain"));

    if (error) {
        server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }

    String light = doc["light"]; 
    int value = doc["value"];

    if (light == "light1") p1 = value;
    else if (light == "fan") p2 = value;
    else if (light == "dehydrator") p3 = value;

    handle_logic();

    handle_lights(); 
}

void scanWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);   // ensure not connected
    delay(100);

    int n = WiFi.scanNetworks();

    if (n == 0) {
        Serial.println("No networks found");
    } else {
        Serial.printf("%d networks found\n", n);
        for (int i = 0; i < n; i++) {
            Serial.printf(
                "%2d: %s  RSSI: %d  %s\n",
                i + 1,
                WiFi.SSID(i).c_str(),
                WiFi.RSSI(i),
                (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "open" : "secured"
            );
        }
    }

    WiFi.scanDelete(); // free memory
}

bool connectToWiFi() {
  for (int i = 0; i < wifiCount; i++) {
    Serial.print("Trying to connect to ");
    Serial.println(ssidList[i]);
    WiFi.begin(ssidList[i], passList[i]);

    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) {
      delay(1000);
      Serial.print(".");
      timeout++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected!");
      Serial.println(WiFi.localIP());
      return true;
    }
    Serial.println("\nFailed to connect.");
  }
  return false;
}

void startAccessPoint() {
  const char* apSSID = "ESP32_AP";
  const char* apPassword = "esp32pass";

  WiFi.softAP(apSSID, apPassword);
  Serial.println("Started AP:");
  Serial.println(WiFi.softAPIP());
}


void setup() {
    Serial.begin(115200);


    dht.begin();

    delay(3000);


    humidity = dht.readHumidity();

    temperature = dht.readTemperature();

    scanWiFi();


    delay(1000);

    if (!connectToWiFi()) {
        int serial_wifi_input_trial = 0;
        bool ok = 0;

        while (serial_wifi_input_trial < 5) {
            String wifi_name = "", wifi_pass = "";

            Serial.println("Wifi name : ");
            while (Serial.available() <= 0) {
                delay(1000);
                Serial.print(".");
            }

            if (Serial.available() > 0) {
                wifi_name = Serial.readStringUntil('\n');
                delay(1000);
            }

            Serial.println("Wifi pass : ");
            
            while (Serial.available() <= 0) {
                delay(1000);
                Serial.print(".");
            }

            if (Serial.available() > 0) {
                wifi_pass = Serial.readStringUntil('\n');
                ok = 1;
            }
            if (ok) {
                if (wifi_name != "exit") {
                    Serial.println("Connecting to : " + wifi_name + " with password : " + wifi_pass);

                    WiFi.disconnect(true);

                    delay(5000);
                    Serial.println("Connecting...");

                    WiFi.begin(wifi_name, wifi_pass);

                    int timeout = 0;
                    while (WiFi.status() != WL_CONNECTED && timeout < 100) {
                        delay(1000);
                        Serial.print(".");
                        timeout++;
                    }

                    if (WiFi.status() == WL_CONNECTED) {
                        Serial.println("\nConnected!");
                        Serial.println(WiFi.localIP());

                        break;
                    }
                    Serial.println("\nFailed to connect.");

                    serial_wifi_input_trial ++;
                }
                else serial_wifi_input_trial = 6;

                ok = 0;
            }
        }

        if (WiFi.status() != WL_CONNECTED) {
            startAccessPoint();

            Serial.println(WiFi.localIP());
        }
    }



    Serial.println("Server on");

    server.on("/", handleRoot);  // Serve page at root URL
    server.on("/sensors", HTTP_GET, handle_hardware);
    server.on("/lights", HTTP_POST, handle_set_lights_from_web);
    server.on("/lights", HTTP_GET, handle_lights);
    
    server.begin();
    Serial.println("Web server started!");

    if (WiFi.getMode() == WIFI_STA && WiFi.status() == WL_CONNECTED) {
        if (!MDNS.begin("esp32")) {
            Serial.println("mDNS failed");
        } else {
            MDNS.addService("http", "tcp", 80);
            Serial.println("mDNS started");
            Serial.println("Open: http://esp32.local");
        }
    } else {
        Serial.println("mDNS skipped (not in STA mode)");
    }



    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);
    pinMode(RELAY3_PIN, OUTPUT);

    p1 = 0;
    p2 = 0;
    p3 = 0;

    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
    digitalWrite(RELAY3_PIN, LOW);

}

void loop() {
    humidity = dht.readHumidity();

    temperature = dht.readTemperature();

    server.handleClient();

    // if (Serial.available() > 0) {
    //     String code = Serial.readStringUntil('\n');

    //     if (code == "change") {
    //         while (Serial.available() <= 0) {
    //             delay(1000);
    //             Serial.print(".");
    //         }

    //         String type, val;

    //         if (Serial.available() > 0) {
    //             type = Serial.readStringUntil('\n');
    //             delay(1000);
    //         }

    //         while (Serial.available() <= 0) {
    //             delay(1000);
    //             Serial.print(".");
    //         }

    //         if (Serial.available() > 0) {
    //             val = Serial.readStringUntil('\n');
    //             delay(1000);
    //         }


    //     }
    // }

    handle_logic();
}
