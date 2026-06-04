#  Sistema Inteligente de Rega Preditiva e Eficiência Hídrica

##  Introdução
A agricultura urbana, a jardinagem e a gestão de pequenas plantações enfrentam um desafio constante: o enorme desperdício de recursos hídricos devido a sistemas de irrigação ineficientes. Modelos tradicionais baseiam-se em temporizadores fixos ou, no máximo, em sensores locais que ativam bombas assim que a terra seca, ignorando completamente o cenário climático iminente. 

Este projeto propõe uma solução de **Internet das Coisas (IoT)** avançada para resolver esse problema. O sistema une a telemetria de borda (*Edge Computing*) com a inteligência na nuvem (*Cloud Computing*), criando um ecossistema que não apenas monitora o solo em tempo real, mas também antecipa o comportamento do clima.

##  Descrição do Projeto
O **Sistema de Rega Inteligente** é uma arquitetura de irrigação de precisão focada no consumo responsável. Em vez de operar sob uma lógica binária simples (ligar/desligar), o motor de regras do sistema atua de forma analítica e complementar. 

O cérebro da aplicação cruza a umidade real do solo com previsões meteorológicas de curtíssimo prazo extraídas via API. Através de algoritmos preditivos, o sistema calcula a contribuição exata que a natureza fornecerá (em milímetros de chuva) e comanda o atuador físico para injetar na terra apenas o déficit exato necessário para manter a saúde da planta. 

Totalmente alinhado com o **ODS 15 (Objetivos de Desenvolvimento Sustentável da ONU - Vida Terrestre)**, este ecossistema demonstra como a integração entre microcontroladores, APIs climáticas e bancos de dados temporais pode transformar a automação agrícola, evitando o sufocamento das raízes por excesso de água e garantindo um manejo hídrico altamente eficiente.

## Link do Video demonstrando o funcionamento do projeto
https://youtu.be/LoRJ8quBXnQ

## 🛠️ Descritivo Técnico das Tecnologias

O ecossistema do projeto foi desenvolvido utilizando tecnologias consolidadas no mercado de Internet das Coisas (IoT), computação em nuvem e análise de dados em tempo real. A integração foi projetada para garantir baixo consumo de banda, assincronismo e alta escalabilidade.

### 1. Camada de Hardware e Simulação (Edge Computing)
* **Microcontrolador ESP32 (Espressif Systems):** Utilizado como o núcleo de processamento local (borda). A escolha do chip baseia-se em sua robustez, baixo custo e conectividade Wi-Fi nativa. No ecossistema, dois módulos operam de forma independente: um dedicado à telemetria analógica (sensor de umidade) e outro ao controle preciso de tempo de ativação da bomba (atuador).
* **Wokwi Simulation:** Plataforma utilizada para a prototipagem e simulação do hardware em nuvem. O ambiente executa o firmware real compilado para a arquitetura do ESP32, simulando com precisão o comportamento dos pinos de GPIO, conexões de rede Wi-Fi e periféricos (LEDs e potenciômetros).,
* **Linguagem C++ (Arduino Core):** Os firmwares foram desenvolvidos em C++ utilizando a IDE do Arduino. A programação emprega técnicas avançadas de controle **não-bloqueante** através da função `millis()`, garantindo que o dispositivo continue processando mensagens de rede (comandos de emergência via MQTT) mesmo enquanto realiza a temporização física da rega.

* Dispositivo Bomba de rega
 https://wokwi.com/projects/465223429449979905

* Dispositivo de controle de umidade
 https://wokwi.com/projects/463589779107248129

### 2. Middleware e Orquestração (Cloud)
* **Node-RED:** Plataforma de desenvolvimento baseada em fluxos. Atua como o middleware analítico central da arquitetura. Suas principais responsabilidades envolvem:
    * Consumo periódico e tratamento sintático de APIs HTTP REST.
    * Gerenciamento de contexto de memória global (`global.set` / `global.get`) para persistir estados entre fluxos assíncronos.
    * Processamento do motor de regras agronômicas através de funções personalizadas em **JavaScript (ECMAScript 6)**.
    * Roteamento dinâmico de dados entre os protocolos HTTP, MQTT e a API nativa do banco de dados.
    <img width="1495" height="648" alt="image" src="https://github.com/user-attachments/assets/5b1d6904-b3d6-4994-b8ff-54ab035ade75" />

### 3. Protocolo e Infraestrutura de Comunicação
* **MQTT (Message Queuing Telemetry Transport):** Protocolo de comunicação de rede baseado no modelo de **Publicação/Assinatura (Pub/Sub)**. Opera sobre a camada TCP/IP e foi escolhido por sua extrema leveza, sobrecarga de rede (*overhead*) mínima e eficiência em redes IoT instáveis.
* **HiveMQ Cloud:** Broker MQTT público utilizado como o servidor intermediário das mensagens. A arquitetura foi isolada utilizando tópicos estruturados de forma hierárquica e geradores de **Client ID aleatórios** nativos no firmware (`esp_random()`), mitigando conflitos de concorrência e quedas de conexão comuns em ambientes compartilhados.

### 4. Armazenamento e Banco de Dados Temporal
* **InfluxDB Cloud (Time Series Database - TSDB):** Banco de dados relacional otimizado especificamente para dados indexados pelo tempo (séries temporais). Diferente de bancos relacionais tradicionais, o InfluxDB é altamente eficiente para lidar com grandes volumes de inserções contínuas de telemetria. 
    * Os dados são estruturados através de campos (*fields*) e medições (*measurements*), permitindo consultas rápidas via linguagem **Flux** para monitorar o status dinâmico dos atuadores e o histórico acumulado de consumo de água.

### 5. Provedor de Dados Climatológicos
* **OpenWeather API (Rota Forecast):** Serviço de microsservidor na nuvem que fornece previsões meteorológicas globais baseadas em coordenadas geográficas. O projeto consome o endpoint de previsão detalhada, extraindo objetos JSON estruturados contendo dados preditivos para as próximas 3 horas, essenciais para alimentar o cálculo de rega complementar preventiva.
