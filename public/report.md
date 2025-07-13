# Sentinel IOC: Analisador de Indicadores de Comprometimento

## 1. Introdução
Cibersegurança tornou-se um pilar fundamental para proteger dados e sistemas. Uma das ferramentas mais eficazes na detecção e prevenção de ataques são os Indicadores de Comprometimento (IOCs). Estes são fragmentos de dados forenses, como endereços IP maliciosos, URLs suspeitas ou hashes de arquivos infectados, que atuam como "impressões digitais" de atividades maliciosas. Este projeto, intitulado "Sentinel IOC", simula um analisador de IOCs, oferecendo uma introdução prática à Programação Orientada a Objetos (POO) utilizando a linguagem C++. Ele permite o cadastro, consulta, edição e exclusão dessas informações, fornecendo uma base para a compreensão de conceitos de POO em um contexto de segurança da informação.

## 2. Objetivos do Projeto
O projeto Sentinel IOC foi desenvolvido com múltiplos objetivos em mente, visando proporcionar uma experiência de aprendizado abrangente e prática:

 -Aprender POO com C++: Implementação Herança, Polimorfismo e Encapsulamento em C++, aplicando-os na modelagem de um sistema real.
 -Simular um analisador de IOCs: Criar uma aplicação que replique as funcionalidades básicas de um sistema de gerenciamento e análise de Indicadores de Comprometimento, familiarizando os   com o fluxo de trabalho de um analista de segurança.
 -Trabalhar com arquivos como base de dados: Desenvolver a capacidade de persistir dados em arquivos de texto (.txt), simulando um banco de dados simples para armazenar e recuperar  informações sobre os IOCs.
 -Explorar boas práticas de código e versionamento: Incentivar o desenvolvimento de código limpo, organizado e legível, além de praticar o uso de sistemas de controle de versão (como Git e GitHub) para colaboração e gerenciamento do projeto.


## 3. Arquitetura do Sistema
O sistema Sentinel IOC é uma aplicação em C++ que permite gerenciar uma lista de Indicadores de Comprometimento. Sua arquitetura é baseada em Programação Orientada a Objetos, utilizando uma hierarquia de classes bem definida para organizar e manipular os dados.

Explicação das Classes:
- Indicador:  Esta é a classe abstrata base do sistema. Ela define os atributos e métodos comuns a todos os tipos de IOCs, como o tipo do indicador (IP, URL, Hash) e seu valor. Além disso, possui uma função virtual pura (exibirInfo()) para exibir informações, forçando as classes derivadas a implementarem sua própria versão específica. Ela serve como um "modelo" para as subclasses, garantindo que todos os indicadores compartilhem características essenciais.
- Classes derivadas: 
    IPMalicioso: Herda da classe Indicador e representa um endereço IP malicioso. Pode incluir atributos extras específicos, como o país de origem do IP.
    URLMaliciosa: Também herda de Indicador e representa uma URL suspeita.
    HashMalicioso: Deriva de Indicador e representa o hash de um arquivo infectado. Pode ter atributos adicionais, como o algoritmo de hashing utilizado.
    Cada uma dessas classes implementa o método exibirInfo() de forma polimórfica, apresentando os detalhes relevantes para seu tipo específico de IOC.
- AnalisadorIOC: Esta classe atua como o controlador principal do sistema. Ela é responsável pela lógica de CRUD (Create, Read, Update, Delete) dos IOCs. Contém um vetor de ponteiros para  Indicador, o que permite armazenar objetos de qualquer uma das classes derivadas de forma polimórfica. Além das operações CRUD, a AnalisadorIOC também gerencia a persistência dos dados, incluindo métodos para salvar e carregar os IOCs de um arquivo de texto, buscar IOCs por tipo ou valor e simular a análise de um IOC.
- Diagrama de classes

    +--------------------+
|        IOC         |  <- Classe base 
+--------------------+
| - id: int          |
| - tipo: string     |
| - valor: string    |
| - severidade: string |
| - origem: string   |
| - data: string     |
| - descricao: string |
+--------------------+
| + getTipo(): string |
| + getValor(): string |
| + getSeveridade(): string |
| + getOrigem(): string |
| + getData(): string |
| + getDescricao(): string |
| + setTipo(t: string): void |
| + setValor(v: string): void |
| + setSeveridade(s: string): void |
| + setOrigem(o: string): void |
| + setData(d: string): void |
| + setDescricao(desc: string): void |
+--------------------+
        ^
        |
+----------------+  +---------------+  +----------------+
|   IOC_IP       |  |   IOC_URL     |  |  IOC_Hash      |
+----------------+  +---------------+  +----------------+
| + metodoEspecifico(): void |   | + metodoEspecifico(): void |   | + metodoEspecifico(): void |
+----------------+  +---------------+  +----------------+

+----------------------+
|      SistemaIOC       |
+----------------------+
| - listaIOCs: vector<IOC*> |
+----------------------+
| + adicionarIOC(ioc: IOC*): void |
| + removerIOC(id: int): void |
| + listarIOCs(): void |
| + listarIOCsFiltrados(): void |
| + buscarIOC(): void |
| + salvarArquivo(): void |
| + carregarArquivo(): void |
+----------------------+

+------------------------------+
|      InterfaceGrafica        |
+------------------------------+
| + mostrarMenu(): void        |
| + capturarEntrada(): void    |
| + mostrarIOCs(lista): void   |
| + exibirMensagem(msg): void  |
+------------------------------+



## 4. Conceitos de POO Utilizados
O projeto Sentinel IOC é uma ferramenta para solidificar o entendimento dos principais conceitos da Programação Orientada a Objetos:
- Herança: É fundamental na estrutura do projeto. As classes IPMalicioso, URLMaliciosa e HashMalicioso herdam da classe base abstrata Indicador. Isso permite reutilizar atributos e métodos comuns e estabelecer uma relação de "é um" (por exemplo, um IPMalicioso é um Indicador), evitando a duplicação de código e organizando a hierarquia de classes.
- Polimorfismo: É intensamente utilizado no AnalisadorIOC. O vetor std::vector<Indicador*> pode armazenar ponteiros para objetos de qualquer uma das classes derivadas (IPMalicioso, URLMaliciosa, HashMalicioso). Quando o método exibirInfo() é chamado para um objeto nesse vetor, o polimorfismo garante que a implementação correta (específica da classe derivada) seja executada em tempo de execução. Isso também se aplica à simulação de análise, onde mensagens diferentes podem ser exibidas dependendo do tipo do IOC.
- Encapsulamento: Os dados (atributos) das classes são protegidos utilizando modificadores de acesso private, e são acessados ou modificados apenas através de métodos públicos (getters e setters). Isso garante a integridade dos dados e impede o acesso direto e não controlado, promovendo a modularidade e a manutenibilidade do código.
- Uso de ponteiros e referências: São empregados para possibilitar o armazenamento polimórfico dos objetos Indicador* no vetor da classe AnalisadorIOC e para manipular objetos de forma eficiente, especialmente ao passar objetos para funções ou retornar valores, evitando cópias desnecessárias e gerenciando a memória de forma mais flexível.


## 5. Funcionalidades Implementadas
O Sentinel IOC oferece um conjunto robusto de funcionalidades, todas acessíveis através de uma interface de linha de comando (CLI), permitindo ao usuário interagir de forma prática com o sistema de gerenciamento de IOCs:
- CRUD completo:
    Create (Adicionar): Permite ao usuário cadastrar novos IOCs, escolhendo entre os tipos (IP Malicioso, URL Maliciosa, Hash Malicioso) e inserindo os dados pertinentes.
    Read (Listar/Buscar): Permite listar todos os IOCs cadastrados, exibindo suas informações detalhadas de forma polimórfica. Também é possível buscar um IOC específico por seu valor.Update (Atualizar): Oferece a funcionalidade de modificar os dados de um IOC existente, selecionando-o e fornecendo as novas informações.
    Delete (Remover): Permite a exclusão de um IOC do sistema, selecionando-o pelo seu valor.
- Busca de IOC por tipo ou valor: Além da listagem geral, o sistema suporta a busca direcionada de IOCs, facilitando a localização de informações específicas.
- Simulação de análise: Uma funcionalidade de "análise" é implementada, que simula uma verificação do IOC. Dependendo do tipo do IOC, mensagens diferentes são exibidas, demonstrando o uso de polimorfismo para comportamentos específicos. Por exemplo, pode indicar se um IP está listado em uma base fictícia de ameaças.
- Interface por linha de comando: Toda a interação com o usuário é feita através de um menu de texto simples no console, tornando o projeto acessível e focado nos conceitos de lógica e POO, sem a complexidade de uma interface gráfica. Ao iniciar, o sistema carrega os dados de um arquivo (iocs.txt), e antes de sair, salva as alterações, garantindo a persistência dos dados.

## 6. Testes Realizados
Para garantir a funcionalidade e robustez do Sentinel IOC, foram realizados testes manuais abrangentes, validando cada uma das funcionalidades implementadas. Os principais cenários testados incluem:
    - Validação da Inicialização:
        Verificação do carregamento correto dos dados de iocs.txt ao iniciar o programa.
        Confirmação de que o programa inicia mesmo com um arquivo iocs.txt vazio ou inexistente.
    - Testes de CRUD:
        Adição (Create):
            Adição de IOCs de todos os tipos (IPMalicioso, URLMaliciosa, HashMalicioso) com diferentes valores e atributos.
            Tentativa de adicionar IOCs com valores inválidos.
    - Listagem (Read):
        Verificação se todos os IOCs adicionados são exibidos corretamente na lista, com seus atributos específicos (exibirInfo() polimórfico).
        Confirmação da ordem de exibição.
    - Busca (Read):
        Busca de IOCs existentes por valor, garantindo que o IOC correto seja retornado.
        Busca por IOCs não existentes, verificando a mensagem de "não encontrado".
    - Atualização (Update):
        Atualização de atributos de IOCs existentes de todos os tipos.
        Verificação se as alterações são refletidas corretamente após a atualização e na listagem.
        Tentativa de atualizar um IOC não existente.
    - Remoção (Delete):
        Remoção de IOCs existentes de diferentes tipos.
        Verificação se o IOC removido não aparece mais na lista.
        Tentativa de remover um IOC não existente.
    - Testes de Persistência:
        Adição, atualização e remoção de IOCs, seguido de "Salvar e sair".
        Reinício do programa para verificar se as alterações foram salvas corretamente e carregadas novamente.
        Verificação do formato do arquivo iocs.txt após as operações.
    - Testes da Simulação de Análise:
        Execução da funcionalidade de "Analisar" para diferentes tipos de IOCs.
        Verificação se as mensagens polimórficas são exibidas corretamente para cada tipo de IOC.
    - Testes de Interação CLI:
        Validação de todas as opções do menu.
        Verificação do tratamento de entradas inválidas no menu.
Os testes foram conduzidos de forma a cobrir os fluxos principais e alternativos do sistema, garantindo que todas as funcionalidades se comportem conforme o esperado e que os conceitos de POO sejam aplicados de maneira correta e eficaz.

## 7. Aprendizados e Desafios
- O que foi mais difícil:
   Gerenciamento de Memória com Ponteiros e Polimorfismo: Um dos maiores desafios foi o correto gerenciamento da memória ao lidar com std::vector<Indicador*>. Garantir que os objetos fossem alocados dinamicamente e desalocados adequadamente para evitar vazamentos de memória, especialmente ao remover IOCs ou ao finalizar o programa, exigiu atenção redobrada. Compreender como os ponteiros para a classe base se comportam com objetos das classes derivadas e a importância do destrutor virtual na classe base foi crucial.

   Dificuldade para Depurar Código: A natureza de baixo nível do C++ e o uso extensivo de ponteiros e alocação dinâmica tornaram a depuração um desafio considerável. Erros de segmentação e vazamentos de memória eram frequentes, exigindo o uso meticuloso de ferramentas de depuração  e uma análise cuidadosa do fluxo de execução para identificar a causa raiz dos problemas, que muitas vezes não eram óbvios ou diretos.

   Organizar a Ordem de Prioridade da Implementação das Funcionalidades: Definir uma sequência lógica para o desenvolvimento das funcionalidades foi um ponto crítico. Decidir o que implementar primeiro – a hierarquia de classes, o CRUD básico, a persistência em arquivo ou a interface de usuário – e como essas partes se integrariam sem gerar bloqueios ou retrabalho excessivo, exigiu um bom planejamento e a capacidade de adaptar a estratégia à medida que os desafios surgiam.

   Implementação Polimórfica Robusta: Assegurar que o polimorfismo funcionasse de maneira consistente em todas as funcionalidades (listagem, busca, análise) foi complexo. Garantir que o método exibirInfo() fosse chamado corretamente para o tipo de objeto subjacente, e que a desserialização do arquivo de texto instanciasse o tipo correto de Indicador (IP, URL ou Hash) dinamicamente, demandou um bom entendimento dos conceitos e da sintaxe de C++.

   Manipulação de Arquivos para Persistência: A lógica de salvar e carregar os dados dos IOCs em um arquivo de texto, mantendo a integridade e o formato correto para posterior leitura, apresentou seus próprios obstáculos. Lidar com a leitura de diferentes atributos para cada tipo de IOC e reconstruir os objetos polimorficamente a partir das strings do arquivo exigiu um design cuidadoso da lógica de serialização/desserialização. 

- O que aprendemos sobre C++ e OOP
   Aprofundamento em Herança e Polimorfismo: O projeto reforçou o entendimento de como a herança permite a reutilização de código e a criação de hierarquias lógicas. O polimorfismo, por sua vez, demonstrou o poder de escrever código genérico que se adapta ao tipo específico do objeto em tempo de execução, tornando o sistema mais flexível e extensível.

   Importância do Encapsulamento: A prática de encapsular os dados e expor apenas métodos controlados para acesso e modificação (getters e setters) provou ser essencial para manter a integridade dos dados e facilitar a manutenção do código.

   Gerenciamento de Ciclo de Vida de Objetos: Houve um aprendizado significativo sobre o ciclo de vida dos objetos em C++, desde a alocação dinâmica com new até a liberação com delete. A necessidade de um destrutor virtual na classe base abstrata para garantir a chamada correta dos destrutores das classes derivadas foi um aprendizado fundamental.

   Tratamento de Exceções e Erros Básicos: Embora não explicitamente detalhado, a implementação de robustez mínima para lidar com entradas inválidas do usuário e erros de arquivo foi um aprendizado prático, mostrando a importância da resiliência do sistema.

## 8. Conclusão
O projeto Sentinel IOC representa aprendizado prático de Programação Orientada a Objetos em C++. Ao simular um analisador de Indicadores de Comprometimento, os participantes não apenas aplicaram conceitos fundamentais como herança, polimorfismo e encapsulamento em um cenário real, mas também desenvolveram habilidades essenciais em manipulação de arquivos para persistência de dados e interação via linha de comando.
Os desafios enfrentados, especialmente no gerenciamento de memória com ponteiros e na implementação polimórfica robusta, foram cruciais para aprofundar a compreensão do C++ e suas particularidades. As soluções encontradas para esses desafios não só fortaleceram o conhecimento técnico, mas também a capacidade de depuração e resolução de problemas.



