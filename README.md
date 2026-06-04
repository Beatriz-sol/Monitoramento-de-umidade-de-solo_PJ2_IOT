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

### 3. Protocolo e Infraestrutura de Comunicação
* **MQTT (Message Queuing Telemetry Transport):** Protocolo de comunicação de rede baseado no modelo de **Publicação/Assinatura (Pub/Sub)**. Opera sobre a camada TCP/IP e foi escolhido por sua extrema leveza, sobrecarga de rede (*overhead*) mínima e eficiência em redes IoT instáveis.
* **HiveMQ Cloud:** Broker MQTT público utilizado como o servidor intermediário das mensagens. A arquitetura foi isolada utilizando tópicos estruturados de forma hierárquica e geradores de **Client ID aleatórios** nativos no firmware (`esp_random()`), mitigando conflitos de concorrência e quedas de conexão comuns em ambientes compartilhados.

### 4. Armazenamento e Banco de Dados Temporal
* **InfluxDB Cloud (Time Series Database - TSDB):** Banco de dados relacional otimizado especificamente para dados indexados pelo tempo (séries temporais). Diferente de bancos relacionais tradicionais, o InfluxDB é altamente eficiente para lidar com grandes volumes de inserções contínuas de telemetria. 
    * Os dados são estruturados através de campos (*fields*) e medições (*measurements*), permitindo consultas rápidas via linguagem **Flux** para monitorar o status dinâmico dos atuadores e o histórico acumulado de consumo de água.

### 5. Provedor de Dados Climatológicos
* **OpenWeather API (Rota Forecast):** Serviço de microsservidor na nuvem que fornece previsões meteorológicas globais baseadas em coordenadas geográficas. O projeto consome o endpoint de previsão detalhada, extraindo objetos JSON estruturados contendo dados preditivos para as próximas 3 horas, essenciais para alimentar o cálculo de rega complementar preventiva.

#  Guia de Replicação: Sistema de Rega Inteligente Preditiva

Bem-vindo ao tutorial de instalação e replicação do Sistema de Rega Inteligente. Este guia passo a passo foi desenhado para que qualquer pessoa consiga recriar a nossa arquitetura IoT (Hardware, Middleware e Cloud) a partir do zero.

---

## 1. Pré-requisitos e Contas Necessárias

Antes de iniciar, certifique-se de ter acesso às seguintes plataformas (todas possuem planos gratuitos ideais para este teste):

| Plataforma | Propósito no Projeto | O que você precisa ter/criar |
| :--- | :--- | :--- |
| **Node-RED** | Cérebro analítico e motor de regras | Instalação local ativa ou instância na nuvem. |
| **Wokwi** | Simulação do hardware (Edge) | Conta gratuita no site wokwi.com. |
| **OpenWeather** | Previsão climática preditiva | Cadastro no site e uma **API Key** ativa. |
| **InfluxDB Cloud** | Banco de dados temporal | Conta na versão Cloud 2.0. |
| **HiveMQ** | Broker MQTT Público | Nenhum cadastro necessário (usaremos o broker público). |

---

##  2. Preparando as Nuvens (API e Banco de Dados)

### Passo 2.1: OpenWeather API
1. Acesse o site do OpenWeather e faça login.
2. Navegue até a aba **API Keys** e copie a sua chave gerada. Reserve esta chave, ela será o "passaporte" do Node-RED para ver a previsão do tempo.

### Passo 2.2: InfluxDB Cloud
1. Acesse o InfluxDB Cloud e vá ao menu lateral **Load Data > Buckets**.
2. Crie um novo bucket e nomeie-o **EXATAMENTE** como: `dados_clima`.
3. Vá ao menu **API Tokens**, clique em *Generate API Token* (All Access) e copie o token gerado.
4. Anote a **URL da sua organização** (o link que aparece no seu navegador, ex: `https://us-east-1-1.aws.cloud2.influxdata.com`).

---

##  3. Montando o Hardware Virtual (Wokwi)

Precisamos criar dois projetos independentes no Wokwi utilizando o microcontrolador **ESP32**.

### Circuito 1: O Atuador (Bomba de Rega)
1. Crie um novo projeto ESP32 no Wokwi.
2. Adicione um **LED** ao circuito (ele representará a nossa bomba de água).
3. Conecte a perna positiva (Anode) do LED na porta **D26** do ESP32, e a perna negativa (Cathode) no **GND**.
4. Cole o código-fonte localizado na pasta `/dispositivo-bomba-de-rega.
5. Certifique-se de que a rede WiFi no código está configurada como `Wokwi-GUEST`.

### Circuito 2: O Sensor (Umidade do Solo)
1. Crie um segundo projeto ESP32 no Wokwi em outra aba do navegador.
2. Adicione um **Potenciômetro** (Slide ou Rotativo) para simular a variação de umidade da terra.
3. Conecte o pino de sinal (SIG) numa porta analógica do ESP32 (ex: **D34**), o VCC no **3V3** e o GND no **GND**.
4. Cole o código-fonte localizado na pasta `/dispositivo-verificador-de-Umidade`.

---

## 4. Importando o Cérebro (Node-RED)

Com o hardware e os serviços de nuvem prontos, vamos conectar tudo.

1. Abra a sua interface do **Node-RED**.
2. Clique no menu superior direito (três linhas) e selecione **Import**.
3. Faça o upload do arquivo `flows.json` que está na pasta `/Codigo-fonte-nodered` deste repositório.
4. O fluxo completo aparecerá na sua tela. Agora, faça as seguintes configurações:
   * **Nó da API (Requisição HTTP):** Dê um duplo clique, encontre o campo da URL e substitua o texto `SUA_API_KEY_AQUI` pela chave do OpenWeather que você anotou no Passo 2.1.
   * **Nós do InfluxDB:** Dê um duplo clique no nó final de gravação do banco de dados, edite o servidor clicando no ícone do lápis e insira a **URL**, **Token** e **Organização** que você gerou no Passo 2.2.
5. Clique no botão vermelho **Deploy** no canto superior direito para salvar e aplicar.

---

## 5. Teste de Ponta a Ponta (Execução)

Para garantir que o fluxo está respeitando a ordem cronológica da memória e o sincronismo dos equipamentos, siga os passos estritamente nesta ordem:

1. **Ligue os Sensores:** Vá até a aba do Wokwi do **Circuito 2 (Umidade)** e clique no botão verde **Play**. Aguarde o Monitor Serial exibir a mensagem "Conectado com sucesso!" e começar a publicar a umidade.
2. **Ligue os Atuadores:** Vá até a aba do Wokwi do **Circuito 1 (Bomba)** e clique em **Play**. Aguarde ele se conectar ao WiFi e ao MQTT.
3. **Popule a Memória:** Aguarde aproximadamente **6 segundos**. Esse é o tempo necessário para o sensor de umidade enviar a primeira leitura e o Node-RED gravá-la em sua memória global.
4. **Dispare a Inteligência:** No Node-RED, clique no botão azul ativador (quadradinho) ao lado do nó de **Inject** (Carimbo de data/hora).
5. **Observe a Mágica:**
   * Se o potenciômetro da umidade estiver **abaixo de 60%**, você verá o LED do Circuito 1 acender automaticamente pelo tempo exato calculado em litros e, ao apagar, enviar a confirmação de fim de rega.
   * Se o potenciômetro estiver **acima de 60%** (ou se houver muita chuva prevista na API), o LED permanecerá desligado, poupando água.
6. **Auditoria:** Abra o seu InfluxDB Cloud, vá ao Data Explorer e visualize o painel atualizando em tempo real com o status de operação (`1` para Regando e `0` para Desligada) e os litros registrados.

---

 **Pronto! O seu ecossistema IoT de eficiência hídrica está operando com sucesso.**


