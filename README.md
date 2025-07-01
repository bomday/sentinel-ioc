# Sentinel IOC

> Projeto de Análise de Indicadores de Comprometimento (IOCs) em C++ utilizando Programação Orientada a Objetos.

## Descrição

Este projeto simula um analisador de IOCs (IP, URLs e Hashes maliciosos), permitindo o cadastro, consulta, edição e exclusão dessas informações. É uma introdução prática à Programação Orientada a Objetos em C++.

## Tecnologias Utilizadas

- Linguagem: C++
- POO: Herança, Polimorfismo, Encapsulamento
- Manipulação de arquivos (.txt)
- Git + GitHub
- GitHub Pages (documentação)
- Ponteiros e Referências

## Funcionalidades

- [X] Cadastro de IOCs (IP, URL, Hash)
- [X] Consulta e listagem de IOCs
- [X] Atualização e exclusão de IOCs
- [X] Simulação de análise
- [X] Interface em linha de comando
- [ ] GUI com biblioteca gráfica (opcional)

## Organização do Código

```
📁 sentinel-ioc/
 ┣ 📁 src/
 ┃ ┣ 📁 data/
 ┃ ┃ ┗ 📄 iocs.csv                  >> Simulação de "banco de dados"
 ┃ ┣ 📁 fileManager                 
 ┃ ┃ ┣ 📄 fileManager.hpp             >> Classe de persistência de dados
 ┃ ┃ ┗ 📄 fileManager.cpp
 ┃ ┣ 📁 indicator                 
 ┃ ┃ ┣ 📄 indicator.hpp             >> Classe base abstrata
 ┃ ┃ ┗ 📄 indicator.cpp              
 ┃ ┣ 📁 indicatorManager
 ┃ ┃ ┣ 📄 indicatorManager.hpp      >> Gerencia o CRUD de IOCs
 ┃ ┃ ┗ 📄 indicatorManager.cpp
 ┃ ┣ 📁 maliciousHash
 ┃ ┃ ┣ 📄 maliciousHash.hpp         >> Classe derivada: Hash malicioso
 ┃ ┃ ┗ 📄 maliciousHash.cpp
 ┃ ┣ 📁 maliciousIP
 ┃ ┃ ┣ 📄 maliciousIP.hpp           >> Classe derivada: IP malicioso
 ┃ ┃ ┗ 📄 maliciousIP.cpp
 ┃ ┣ 📁 maliciousURL
 ┃ ┃ ┣ 📄 maliciousURL.hpp          >> Classe derivada: URL maliciosa
 ┃ ┃ ┗ 📄 maliciousURL.cpp
 ┃ ┣ 📁 utils
 ┃ ┃ ┣ 📄 utils.hpp                 >> Funções auxiliares (ex: timestamp, menu)
 ┃ ┃ ┗ 📄 utils.cpp
 ┃ ┣ 📄 sentinel.exe                >> Executável (gerado)  
 ┃ ┗ 📄 main.cpp                    >> Ponto de entrada do programa
 ┣ 📁 public/                       
 ┃ ┣ 📄 roteiro-video.pdf           
 ┃ ┣ 📄 relatorio.pdf             
 ┣ 📄 .gitignore
 ┗ 📄 README.md
```

## Conceitos POO Utilizados

- Classes e Objetos: São os blocos de construção fundamentais. As classes (`Indicator`, `MaliciousIP`, `MaliciousURL`, `MaliciousHash`, `IndicatorManager`) definem a estrutura e o comportamento dos objetos, enquanto os objetos são instâncias concretas dessas classes;
- Herança: Permite que classes (`MaliciousIP`, `MaliciousURL`, `MaliciousHash`) herdem características (atributos e métodos) da classe base (`Indicator`). Isso promove o reuso de código e estabelece uma relação de "é um" entre as classes derivadas e a base;
- Polimorfismo: A capacidade de objetos de diferentes classes responderem de forma específica à mesma chamada de método. No exemplo, embora não haja métodos virtuais puros na classe base, o conceito de criar diferentes tipos de indicadores (`IP`, `URL`, `Hash`) e tratá-los de forma genérica através da classe `Indicator` demonstra o princípio do polimorfismo.
- Encapsulamento (acesso com `private`, `protected`, `public`)
- Abstração: A classe `Indicator` pode ser considerada uma classe abstrata (embora não use a palavra-chave virtual para métodos puros), pois define uma interface comum para diferentes tipos de indicadores, sem se preocupar com os detalhes específicos de cada um. As classes derivadas implementam os detalhes específicos.

## Rodando o Programa 
> Dentro do /src
#### Compile o programa (se necessário)
```bash
g++ -std=c++17 -o sentinel.exe     main.cpp     utils/utils.cpp     indicator/indicator.cpp     maliciousIP/maliciousIP.cpp     maliciousURL/maliciousURL.cpp     maliciousHash/maliciousHash.cpp     indicatorManager/indicatorManager.cpp     fileManager/fileManager.cpp     -I.     -Iindicator     -Iutils     -ImaliciousIP     -ImaliciousURL     -ImaliciousHash     -IindicatorManager     -IfileManager
```
#### Execute o programa
```bash
./sentinel.exe
```

## 📹 Vídeo

🔗 Em breve: [Link para o YouTube](#)

## 📄 Documentação

🔗 Em breve: [GitHub Pages](#)

## 👥 Equipe

| Nome           | Curso              |
|--------------|---------------------|
| Beatriz Helena | Sistemas de Informação |
| Dayane Lima | Sistemas de Informação |
| Maria Antônia | Sistemas de Informação |
| Rafael Theles | Sistemas de Informação |
| Williams Andrade | Sistemas de Informação |
