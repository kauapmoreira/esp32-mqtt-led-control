// exercicio
// em dupla estabelecer uma comunicação entre esp's. utilizando o broker MQTT
// um esp deve enviar mensagem para o outro e vice versa
// obs: cada dupla deve criar seus proprios topicos para comunicação
// exercicio etapa 2
// enviar dentro do json a informacao de data e hora que a mensagem foi enviada
// exercicio etapa 3
// controlar o LED (pin 2) do ESP da dupla

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "internet.h"
#include <ezTime.h>
// configura o mqtt
const char *mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char *mqtt_client_id = "senai134_esp_kaua";
const char *mqtt_topic_sub = "senai134/DevKp/pub";
const char *mqtt_topic_pub = "senai_duoKP/pub";
const int pinLed = 2;
// variaveis
String dataHoraCompleta;

void conectaMqtt();
void retornoMqtt(char *topic, byte *payload, unsigned int lenght);
void defineDados();
WiFiClient espClient;
PubSubClient client(espClient);
Timezone sp;

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(5000);
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, LOW);
    // conecta o wifi
    conexaoWifi();

    waitForSync();

    sp.setLocation("America/Sao_Paulo");

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(retornoMqtt);
}

void loop()
{
    // reconecta o wifi
    reconexaoWifi();

    client.loop();
    if (!client.connected())
    {
        conectaMqtt();
    }

    //-------------le da serial
    if (Serial.available() > 0)
    {
        String textoDigitado = Serial.readStringUntil('\n');
        textoDigitado.trim();

        if (textoDigitado.length() == 0)
        {
            Serial.println("Mensagem vazia");
            return;
        }

        defineDados();

        //--------------montar o JSON
        JsonDocument doc;
        doc["disp"] = "esp-kaua";
        doc["msg"] = textoDigitado;
        doc["data"] = dataHoraCompleta;

        String StringJson;
        serializeJson(doc, StringJson);

        // publica no MQTT
        client.publish(mqtt_topic_pub, StringJson.c_str());

        Serial.print("Enviado: ");
        Serial.println(StringJson);
    }
}

void conectaMqtt()
{
    while (!client.connected())
    {
        Serial.println("Conectando ao MQTT");
        if (client.connect(mqtt_client_id))
        {
            Serial.println("Conectado com Sucesso!");
            client.subscribe(mqtt_topic_sub);
        }
        else
        {
            Serial.println("Não houve Conexão");
            Serial.println(client.state());
            Serial.println("Tentando fazer a reconexão novamente");
            delay(5000);
        }
    }
}

void retornoMqtt(char *topic, byte *payload, unsigned int lenght)
{
    Serial.print("Mensagem recebida em ");
    Serial.print(topic);
    Serial.print(": ");

    String msgRecebida = "";
    for (int i = 0; i < lenght; i++)
    {
        msgRecebida += (char)payload[i];
    }

    Serial.println(msgRecebida);

    // ----------interpretar o JSON
    JsonDocument doc;
    DeserializationError erro = deserializeJson(doc, msgRecebida);
    if (erro)
    {
        Serial.print("erro ao decodificar o JSON: ");
        Serial.println(erro.c_str());
        return;
    }

    //------------------acessar os campos do JSON
    const char *dispositivo = doc["disp"];
    const char *msg = doc["msg"];
    const char *data_hora = doc["data"];
    Serial.println("dados decodificados");
    Serial.print("esp: ");
    Serial.println(dispositivo);
    Serial.print("mensagem: ");
    Serial.println(msg);
    if (data_hora)
    {
        Serial.print("enviado as: ");
        Serial.println(data_hora);
    }
    if (msg)
    {
        String comando = String(msg);
        if (comando == "liga")
        {
            digitalWrite(pinLed, HIGH);
        }
        else if (comando == "desliga")
        {
            digitalWrite(pinLed, LOW);
        }
    }   

    Serial.println("-------------------------------");
}

void defineDados()
{
    //================================================//
    dataHoraCompleta = sp.dateTime("d/m/Y H:i:s");
    //================================================//
}