# 🍷 Sistema de Monitoramento Ambiental – Vinheria Agnello

## 📘 Descrição

Projeto desenvolvido para monitorar variáveis ambientais em ambientes de armazenamento de vinhos, utilizando **Arduino Uno** e simulação no **Tinkercad**. O sistema controla **temperatura**, **luminosidade** e **umidade**, garantindo condições ideais para preservar a qualidade dos produtos da Vinheria Agnello.

## 👥 Equipe

- Maria Laura Druzeic  
- Giovanna Dias  
- Marianne Mukai

## 🎯 Objetivo

Criar um sistema embarcado que:
- Leia sensores ambientais em tempo real;
- Exiba dados no display LCD;
- Acione LEDs e buzzer conforme os limites definidos;
- Simule o funcionamento completo no Tinkercad.

## 🧰 Componentes

- Arduino Uno  
- Sensor LM35 (temperatura)  
- LDR (luminosidade)  
- Potenciômetro (simulando umidade)  
- Display LCD 16x2  
- LEDs (verde, amarelo, vermelho)  
- Buzzer  
- Protoboard, resistores e jumpers

## ⚙️ Funcionalidades

- Alternância automática de dados no LCD usando `millis()`  
- Conversão de dados analógicos em °C, lux e %  
- Alertas visuais e sonoros conforme faixas de segurança  
- Funções personalizadas para leitura e exibição centralizada

## 💻 Tecnologias

- Arduino IDE (C++)  
- Tinkercad (simulação)  
- Bibliotecas: `LiquidCrystal`, funções com `map()` e `millis()`

## 🧪 Desafios Técnicos

- Calibração dos sensores  
- Controle de tempo sem `delay()`  
- Exibição clara e dinâmica das informações no LCD

## 📽️ Demonstração

O projeto foi simulado no Tinkercad e apresentado em vídeo, mostrando o funcionamento completo do sistema e sua resposta às variações ambientais.

##Link do vídeo no YouTube: https://youtu.be/ldMSibMshQs?si=EvnHPhwCTO0rBhNP

## 📚 Contexto Acadêmico

Projeto desenvolvido para a disciplina **Edge Computing – CP01**, com foco em sistemas embarcados e IoT.
