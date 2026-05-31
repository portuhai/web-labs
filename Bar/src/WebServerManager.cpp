#include "WebServerManager.h"
#include "BarLogics.h"
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

const char *ssid = "Wokwi-GUEST";
const char *password = "";

WebServer server(80);
  
void handleRoot() {
    if (LittleFS.exists("/index.html")) {
        File file = LittleFS.open("/index.html", "r");
        server.streamFile(file, "text/html");
        file.close();
    } else {
        server.send(404, "text/plain", "Error: index.html not found in LittleFS!");
    }
}

String generateMenuJson() {
    String json = "[";
    for (int i = 0; i < totalMenuItems; i++) {
        json += "{\"id\":" + String(local_db_cocktails[i].id) + ",";
        json += "\"name\":\"" + local_db_cocktails[i].name + "\",";
        json += "\"description\":\"" + local_db_cocktails[i].description + "\"}";
        if (i < totalMenuItems - 1) json += ",";
    }
    json += "]";
    return json;
}

void handleGetMenu() {
    server.send(200, "application/json", generateMenuJson());
}

void handleOrder() {
    if (!server.hasArg("id")) {
        server.send(400, "text/plain", "Bad Request");
        return;
    }

    int id = server.arg("id").toInt();
    if (currentState != MENU) {
        server.send(200, "text/plain", "Bar is already making another order");
        return;
    }

    if (digitalRead(pinCup) == HIGH) {
        startPouringSequence(id, "Web");
        server.send(200, "text/plain", "Order accepted");
    } else {
        server.send(200, "text/plain", "Error: Make sure cup is placed");
    }
}

String generateLogsJson() {
    String json = "[";
    for (int i = 0; i < totalOrdersMade; i++) {
        json += "{\"order_id\":" + String(local_db_orders_log[i].order_id) + ",";
        json += "\"cocktail_id\":" + String(local_db_orders_log[i].cocktail_id) + ",";
        json += "\"cocktail_name\":\"" + getCocktailNameByID(local_db_orders_log[i].cocktail_id) + "\",";
        json += "\"source\":\"" + local_db_orders_log[i].source + "\",";
        json += "\"timestamp\":" + String(local_db_orders_log[i].timestamp_ms) + "}";
        if (i < totalOrdersMade - 1) json += ",";
    }
    json += "]";
    return json;
}

void handleGetLogs() {
    server.send(200, "application/json", generateLogsJson());
}

void handleDownloadLogs() {
    if (LittleFS.exists("/orders.csv")) {
        File file = LittleFS.open("/orders.csv", "r");
        server.sendHeader("Content-Disposition", "attachment; filename=orders.csv");
        server.streamFile(file, "text/csv");
        file.close();
    } else {
        server.send(404, "text/plain", "Empty");
    }
}

void initWebServer() {
    if (!LittleFS.begin(true)) {
        Serial.println("An Error has occurred while mounting LittleFS");
    } else {
        Serial.println("LittleFS mounted successfully");
    }

    WiFi.begin(ssid, password);

    server.on("/", handleRoot);
    server.on("/get-menu", handleGetMenu);
    server.on("/order", handleOrder);
    server.on("/get-logs", handleGetLogs);
    server.on("/download-logs", handleDownloadLogs);
    server.begin();
}

void handleWebServer() {
    server.handleClient();

    static bool connectedReported = false;
    if (WiFi.status() == WL_CONNECTED && !connectedReported) {
        Serial.print("\nWiFi Connected IP: ");
        Serial.println(WiFi.localIP());
        connectedReported = true;
    }
}