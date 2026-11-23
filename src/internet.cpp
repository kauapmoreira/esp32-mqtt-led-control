#include <Arduino.h>
#include <WiFi.h>
#include "internet.h"
#include "senha.h"

const unsigned long tempoConexao = 10000;
const unsigned long tempoReconexao = 5000;

void conexaoWifi()
{
    // conexão wifi
    Serial.printf("Conectando ao wifi: %s", SSID); // permite que misture texto com valor de variaveis|| %s substitui o valor pelo oque ta depois da virgula

    WiFi.begin(SSID, SENHA); // ele vai iniciar o wifi da placa, e vai conectar na rede

    unsigned long tempoWifi = millis();

    // codigo pra esperar conectar
    while (WiFi.status() != WL_CONNECTED && millis() - tempoWifi < tempoConexao)
    {
        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Wifi Conectado Com Sucesso!");
        Serial.print("Endereço De IP: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("Falha Ao Se Conectar");
    }
}

void reconexaoWifi()
{
    unsigned long tempoAtual = millis();
    unsigned long tempoUltimaConexao = 0;

    if (tempoAtual - tempoUltimaConexao > tempoReconexao)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("Conexão Wifi Perdida");
            conexaoWifi();
        }
        tempoUltimaConexao = tempoAtual;
    }
}