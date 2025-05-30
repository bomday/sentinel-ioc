# Sentinel IOC

> Projeto de Análise de Indicadores de Comprometimento (IOCs) em C++ utilizando Programação Orientada a Objetos.

## 📌 Descrição

Este projeto simula um analisador de IOCs (IP, URLs e Hashes maliciosos), permitindo o cadastro, consulta, edição e exclusão dessas informações. É uma introdução prática à Programação Orientada a Objetos em C++.

## 👨‍💻 Tecnologias Utilizadas

- Linguagem: C++
- POO: Herança, Polimorfismo, Encapsulamento
- Manipulação de arquivos (.txt)
- Git + GitHub
- GitHub Pages (documentação)
- Ponteiros e Referências

## 🚀 Funcionalidades

- [ ] Cadastro de IOCs (IP, URL, Hash)
- [ ] Consulta e listagem de IOCs
- [ ] Atualização e exclusão de IOCs
- [ ] Simulação de análise
- [ ] Interface em linha de comando
- [ ] GUI com biblioteca gráfica (opcional)

## 🏗️ Organização do Código

```
📁 sentinel-ioc/
 ┣ 📁 src/
 ┃ ┣ 📄 main.cpp
 ┃ ┣ 📄 Indicador.hpp / Indicador.cpp         (Classe abstrata base)
 ┃ ┣ 📄 IPMalicioso.hpp / IPMalicioso.cpp     (Classe derivada)
 ┃ ┣ 📄 URLMaliciosa.hpp / URLMaliciosa.cpp   (Classe derivada)
 ┃ ┣ 📄 HashMalicioso.hpp / HashMalicioso.cpp (Classe derivada)
 ┃ ┣ 📄 AnalisadorIOC.hpp / AnalisadorIOC.cpp (Classe que contém CRUD e análise)
 ┃ ┣ 📄 utils.hpp / utils.cpp                 (Funções auxiliares)
 ┃ ┗ 📁 data/
 ┃   ┗ 📄 iocs.txt                            (Arquivo simulando o banco de dados)
 ┣ 📁 public/
 ┃ ┣ 📄 roteiro-video.txt 
 ┃ ┣ 📄 relatorio.pdf
 ┗ 📄 README.md
```

## 🧠 Conceitos POO Utilizados

- Classes e Objetos
- Herança (classe base `Indicador`)
- Polimorfismo (métodos sobrescritos)
- Encapsulamento (acesso com `private`, `protected`, `public`)
- Uso de ponteiros e referências

## 📹 Vídeo

🔗 Em breve: [Link para o YouTube](#)

## 📄 Documentação

🔗 Em breve: [GitHub Pages](#)

## 👥 Equipe

| Nome           | Função              |
|--------------|---------------------|
| Aluno 1         | Arquitetura / Dev   |
| Aluno 2        | Backend / CRUD      |
| Aluno 3        | Interface / Docs    |
| Aluno 4        | Interface / Docs    |
| Aluno 4        | Backend / CRUD      |
