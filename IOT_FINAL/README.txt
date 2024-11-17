Descrição Geral do Projeto: Este projeto é um sistema de controle automatizado de temperatura e umidade para armazéns industriais, desenvolvido com a plataforma NodeMCU ESP8266 e sensores de temperatura e umidade (DHT22). Utilizando o protocolo MQTT para comunicação, o sistema monitora o ambiente em tempo real e ajusta automaticamente as condições para manter a integridade dos produtos armazenados.

Objetivo: O objetivo é criar uma solução IoT de baixo custo que permita o monitoramento contínuo e o ajuste das condições ambientais, minimizando a necessidade de supervisão manual e reduzindo os custos operacionais.

Como Reproduzir:

Conecte o NodeMCU ao sensor DHT22 e ao módulo de relé, que será responsável por controlar o ventilador.
Suba o código para o NodeMCU e conecte-o ao Wi-Fi.
Configure o broker MQTT (IP e porta) no código.
Monitore o ambiente através de um painel MQTT, e o sistema ajustará automaticamente a ventilação baseada na leitura da temperatura.
ii) Software Desenvolvido e Documentação de Código
Descrição do Código: O código para este projeto foi escrito em C++ para a plataforma NodeMCU ESP8266 e usa as bibliotecas ESP8266WiFi.h, PubSubClient.h, e DHT.h para Wi-Fi, comunicação MQTT, e leituras do sensor DHT22, respectivamente. Abaixo estão as principais partes do código:

Conexão com o Wi-Fi: Realizada na função setup_wifi, que tenta conectar ao roteador e reinicia o ESP8266 caso a conexão falhe.
Leitura do Sensor DHT22: A cada 2 segundos, o sistema lê a temperatura e umidade e publica esses valores nos tópicos MQTT armazem/temperatura e armazem/umidade.
Controle do Relé: Se a temperatura exceder 25°C, o relé é ativado, ligando o ventilador. Quando a temperatura volta ao limite aceitável, o relé é desligado automaticamente.
Função reconnect: Garante que o dispositivo se reconecte ao broker MQTT em caso de desconexão.
Instruções para Uso do Código:

Baixe as bibliotecas necessárias e carregue o código no ESP8266.
Ajuste o valor do limite de temperatura (atualmente 25°C) no código para o ambiente desejado.
Conecte-se ao painel MQTT para visualizar as leituras em tempo real e os status do sistema.
iii) Descrição do Hardware Utilizado
Componentes Principais:

NodeMCU ESP8266: Utilizado como controlador principal e responsável pela conexão à internet. Ideal para IoT por seu baixo custo e conectividade Wi-Fi integrada.
Sensor DHT22: Monitora a temperatura e umidade. Oferece precisão de ±0.5°C e ±2%, respectivamente.
Módulo Relé de 5V: Controla o ventilador. O relé é acionado com base nas leituras do sensor DHT22, mantendo as condições ambientais ideais no armazém.
Outros Componentes:

Fonte de Alimentação 5V: Alimenta o NodeMCU e o relé.
Ventilador: Dispositivo acionado automaticamente para regular a temperatura. Pode ser substituído por sistemas de ar condicionado em implementações maiores.
Protoboard e Cabos: Utilizados para a montagem do protótipo.
Esquema de Montagem: A montagem do circuito foi feita no software Fritzing. O sensor DHT22 é conectado ao pino D3 do NodeMCU, e o módulo de relé é conectado ao pino D1, responsável pelo controle do ventilador.

iv) Documentação das Interfaces, Protocolos e Módulos de Comunicação
Protocolo Utilizado – MQTT: O MQTT (Message Queuing Telemetry Transport) é utilizado como protocolo de comunicação para a transmissão de dados em tempo real. Ele permite que o NodeMCU envie informações sobre temperatura e umidade a um broker MQTT, e que as ações dos atuadores sejam executadas remotamente.

Configuração do Broker MQTT:

O broker MQTT, como o Mosquitto, deve ser configurado com o IP e porta apropriados.
O código foi configurado para usar a porta padrão 1883.
Tópicos MQTT:

armazem/temperatura: Publica a temperatura ambiente lida pelo sensor.
armazem/umidade: Publica a umidade ambiente lida pelo sensor.
armazem/status: Indica o estado do ventilador (Ligado ou Desligado).
Fluxo de Comunicação:

O NodeMCU coleta dados do sensor DHT22 e os publica no broker MQTT.
Com base nos limites definidos (25°C para temperatura), o NodeMCU controla o estado do relé.
O relé liga/desliga o ventilador automaticamente, mantendo o ambiente dentro dos parâmetros desejados.
v) Comunicação/Controle via Internet (TCP/IP) e Uso do Protocolo MQTT
Este projeto faz uso do protocolo TCP/IP para conexão à internet, o que possibilita ao NodeMCU ESP8266 conectar-se a uma rede Wi-Fi e interagir com o broker MQTT.

Resumo das Etapas de Comunicação:

Conexão à Rede Wi-Fi: O NodeMCU se conecta à rede Wi-Fi local usando o protocolo TCP/IP.
Comunicação MQTT: Os dados de temperatura e umidade são publicados em tópicos MQTT. A comunicação é baseada no modelo de publicação/assinatura, ideal para monitoramento em tempo real e controle remoto.
Controle Remoto: O estado do ventilador (ligado/desligado) pode ser monitorado via MQTT, permitindo ajustes e monitoramento à distância.
