# Comunicação Bidirecional entre ESP32 via MQTT

Projeto desenvolvido em dupla para o curso do **SENAI (Turma 134)** com o objetivo de estabelecer comunicação em tempo real entre dois microcontroladores ESP32 utilizando o protocolo **MQTT**, formato **JSON**, sincronização de data/hora via **NTP** e controle remoto de hardware (LED).

---

## 📋 Etapas do Exercício

1. **Etapa 1 — Comunicação via MQTT:** Estabelecer troca de mensagens bidirecional entre dois ESPs utilizando um broker MQTT público e tópicos exclusivos da dupla.
2. **Etapa 2 — Data e Hora no JSON:** Incluir no payload JSON a data e a hora exatas em que a mensagem foi enviada (fuso horário de São Paulo).
3. **Etapa 3 — Controle Remoto de LED:** Permitir ligar e desligar o LED interno (GPIO 2) do ESP da dupla através de comandos enviados pelo Monitor Serial.

---

## 🛠️ Tecnologias e Bibliotecas

- **Hardware:** ESP32 (utilizando o LED interno no pino `2`)
- **Framework:** Arduino / PlatformIO
- **Broker MQTT:** `broker.emqx.io` (porta `1883`)
- **Bibliotecas utilizadas:**
  - `WiFi.h` — Conexão à rede Wi-Fi (gerenciada pelo cabeçalho auxiliar `internet.h`)
  - `PubSubClient.h` — Cliente MQTT para publicação e assinatura de tópicos
  - `ArduinoJson.h` — Montagem e leitura dos pacotes JSON
  - `ezTime.h` — Sincronização de relógio via internet (NTP) e ajuste de fuso horário (`America/Sao_Paulo`)

---

## 📡 Configuração MQTT

| Parâmetro | Valor configurado |
| :--- | :--- |
| **Broker** | `broker.emqx.io` |
| **Porta** | `1883` |
| **Client ID** | `senai134_esp_kaua` |
| **Tópico de Inscrição (Sub)** | `senai134/DevKp/pub` |
| **Tópico de Publicação (Pub)** | `senai_duoKP/pub` |

> **Observação:** Para que a comunicação funcione em dupla, o segundo ESP deve ter os tópicos **invertidos** (publicar no tópico em que este ESP está inscrito e vice-versa) e um `mqtt_client_id` único.

---

## 📦 Formato do JSON

As mensagens enviadas e recebidas seguem a estrutura abaixo:

```json
{
  "disp": "esp-kaua",
  "msg": "liga",
  "data": "23/11/2026 10:54:00"
}
