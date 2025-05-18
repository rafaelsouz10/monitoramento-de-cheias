# Estação de Monitoramento de Cheias

---

## Descrição do Projeto
sistema embarcado multitarefa, baseado em FreeRTOS, para monitoramento do nível de água e volume de chuva com simulação via joystick analógico. O sistema exibe os dados em um display OLED, ativa sinais visuais e sonoros em situações de alerta e apresenta um painel gráfico na matriz de LEDs WS2812B com símbolos intuitivos.

---

## Requisitos

- **Microcontrolador**: Raspberry Pi Pico ou Raspberry Pi Pico W (opcional)
- **Placa de Desenvolvimento:** BitDogLab (opcional).
- **Editor de Código**: Visual Studio Code (VS Code).
- **SDK do Raspberry Pi Pico** configurado no sistema.
- Ferramentas de build: **CMake** e **Ninja**.

---

## Instruções de Uso

### 1. Clone o Repositório

Clone o repositório para o seu computador:
```bash
git clone https://github.com/rafaelsouz10/monitoramento-de-cheias.git
cd monitoramento-de-cheias
code .
```
---

### 2. Instale as Dependências

Certifique-se de que o SDK do Raspberry Pi Pico está configurado corretamente no VS Code. As extensões recomendadas são:

- **C/C++** (Microsoft).
- **CMake Tools**.
- **Raspberry Pi Pico**.

---

### 3. Configure o VS Code

Abra o projeto no Visual Studio Code e siga as etapas abaixo:

1. Certifique-se de que as extensões mencionadas anteriormente estão instaladas.
2. No terminal do VS Code, compile o código clicando em "Compile Project" na interface da extensão do Raspberry Pi Pico.
3. O processo gerará o arquivo `.uf2` necessário para a execução no hardware real.

---

### 4. Teste no Hardware Real

#### Utilizando a Placa de Desenvolvimento BitDogLab com Raspberry Pi Pico W:

1. Conecte a placa ao computador no modo BOTSEEL:
   - Pressione o botão **BOOTSEL** (localizado na parte de trás da placa de desenvolvimento) e, em seguida, o botão **RESET** (localizado na frente da placa).
   - Após alguns segundos, solte o botão **RESET** e, logo depois, solte o botão **BOOTSEL**.
   - A placa entrará no modo de programação.

2. Compile o projeto no VS Code utilizando a extensão do [Raspberry Pi Pico W](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico):
   - Clique em **Compile Project**.

3. Execute o projeto clicando em **Run Project USB**, localizado abaixo do botão **Compile Project**.

---

### 🔌 5. Conexões e Esquemático
Abaixo está o mapeamento de conexões entre os componentes e a Raspberry Pi Pico W:

| **Componentes**        | **Pino Conectado (GPIO)** |
|------------------------|---------------------------|
| Display SSD1306 (SDA)  | GPIO 14                   |
| Display SSD1306 (SCL)  | GPIO 15                   |
| Joystick (ADC1)        | GPIO 26                   |
| Joystick (ADC0)        | GPIO 27                   |
| Display SSD1306 (SCL)  | GPIO 15                   |
| Matriz LED RGB         | GPIO 7                    |
| LED RGB Vermelho       | GPIO 13                   |
| LED RGB Verde          | GPIO 11                   |
| Buzzer                 | GPIO 21                   |
| Botão B                | GPIO 6                    |

#### 🛠️ Hardware Utilizado
- **Microcontrolador Raspberry Pi Pico W**
- **Display OLED SSD1306 (I2C)**
- **Botão B BOOTSEL**
- **Buzzer**
- **LED RGB**
- **FreeRTOS**

---

### 6. Funcionamento do Sistema

#### 📌 Funcionalidades

**O projeto monitora dois parâmetros simulados:**

- **Nível da água**

- **Volume de chuva**

Cada parâmetro é lido a partir dos eixos de um **joystick** e convertido em **porcentagem**. Com base nos valores, o sistema pode operar em dois modos:

- **Modo Normal:**

    - Exibe os dados no **display OLED**, sinaliza com **LED verde** e mantém os demais dispositivos inativos.

- **Modo Alerta:**

    Ativado se:

        - **Nível da água ≥ 70%**

        - **Volume de chuva ≥ 80%**

    Neste modo:

    - **LED RGB fica vermelho**

    - **Buzzer** emite **padrões sonoros** distintos por tipo de alerta

    - A **matriz WS2812** exibe símbolos **animados/piscantes:**

        - Exclamação vermelha **(alerta geral)**

        - Copo enchendo **(nível de água)**

        - Gota piscando **(chuva)**

---

### 7. Vídeos Demonstrativo

**Click [AQUI](LINK DO VÍDEO) para acessar o link do Vídeo Ensaio**