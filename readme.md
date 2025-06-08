# 🌦 DisasterShield - Monitoramento de enchentes e deslizamentos
Projeto desenvolvido de forma autônoma com fins educacionais, durante meus estudos em **Engenharia da Computação**. Utilizando o arduíno UNO foi montado um **sistema inteligente de alertas para enchentes e deslizamentos de terra**, integrando *sensores de solo, ultrassônicos*, um *display LCD I2C 16x2* e um *buzzer* para avisar quando o nível for crítico. 

## 👩‍💻 Autora
* [Mariana Lara](https://github.com/larads) 

## 💻 Desenvolvimento

### 🔧 Componentes Utilizados
* 1x **Arduino UNO**
* 1x **Sensor de umidade do solo (analógico)**
* 1x **Sensor ultrassônico HC-SR04**
* 1x **Buzzer**
* 1x **Display LCD I2C 16x2**
* 1x **LED RGB (pinos PWM)**
* **Jumpers e Protoboard**

### ⚙️ Funcionalidades do Circuito
* **Medição do nível da água** com *sensor ultrassônico*
* **Leitura da umidade do solo** com *sensor próprio* para *prever risco* de deslizamento
* Exibição das leituras no *display LCD* com **mensagens contextualizadas**
* Ativação de **alerta sonoro** com o *buzzer* e **visual** com o *LED RGB* em caso de **risco iminente**
    * <font color="lightgreen">LED Verde</font>: Seguro
    * <font color="yellow">LED Amarelo</font>: Alerta
    * <font color="red">LED Vermelho</font>: Crítico

### 🧠 Lógica de Avaliação
| Condição | Estado | Ação |
| ----------- | ----------- | ----------- |
| 💧 Nível da Água (< 1.5m) | Nível Ideal | <font color="lightgreen">LED Verde</font>
| 💧 Nível da água (< 2.5m)   | Nível Ok |  <font color="yellow">LED Amarelo</font> + alerta LCD
| 💧 Nível da água (> 2.5m)      | Nível Crítico | <font color="red">LED Vermelho</font> + buzzer + alerta LCD
| 🪴 Solo seco (< 400) | Solo Ideal | <font color="lightgreen">LED Verde</font>
| 🪴 Umidade (< 650) | Solo Úmido | <font color="yellow">LED Amarelo</font> + alerta LCD
| 🪴 Umidade do solo > 650 | Solo Molhado | <font color="red">LED Vermelho</font> + buzzer + alerta LCD

### 🏧 Lógica de Máquina de Estados

O sistema utilizou a lógica de **máquinas de estado** para organizar e manipular o funcionamento do código, facilitando sua **manutenção e expansão futura**. O funcionamento foi dividido em **3 etapas principais**:

#### 1. **Atribuição de valores dos sensores**  
Atribui os dados retornados do **sensor ultrassônico** (nível da água) e do **sensor de umidade do solo** a **variáveis globais**, que serão utilizadas na etapa de análise.

---

#### 2. **Análise dos valores atribuídos**  
Verifica os valores coletados e **exibe no display LCD** o **estado atual** do nível da água e da umidade do solo, classificando como **ideal**, **alerta** ou **crítico**.

---

#### 3. **Alarme de estados críticos e de alerta**  
Caso seja identificado um risco de **enchente ou deslizamento**, o sistema emite um **alarme sonoro (buzzer)** e **visual (LED RGB ou mensagens no display LCD)** para notificar o usuário.

## ✅ Considerações Finais
Este projeto destaca o papel fundamental da tecnologia como ferramenta estratégica para a preservação de vidas em áreas de risco, especialmente em um país como o Brasil, onde eventos climáticos extremos têm se tornado mais frequentes. A implementação de soluções como o **DisasterShield** demonstra que, por meio de sistemas simples, de baixo custo e facilmente escaláveis, é possível promover segurança, antecipar desastres e gerar um impacto social significativo. Iniciativas como esta evidenciam o potencial transformador da engenharia aplicada à realidade das comunidades vulneráveis.