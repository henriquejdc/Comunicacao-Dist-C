# Comunicação entre Nodos utilizando C

Descrição:
- Inicie e espera um tempo para conversão dos roteadores ligados, e o mesmo caso tire um roteador ligado.

Structs:
- Roteador struct que recebe informação destes
- Enlace struct que recebe informação do mesmos
- Lg struct que guarda um mode de enlace para a struct vt
- Vt struct dos vetores de cada roteador
- Tb struct da tabela dos vetores que são compostos por lg, os enlaces
- Mensage struct original para mensagem do trabalho 1, usada para mensagens de usuário
- Message_s struct para troca de vetores e suas composições, usada para controle da tabela

Funções:
- Nodos lg, vt e tb são funções para criar um novo malloc destes tipos de nodos.
- Mostra1 é uma função generica para ver composição de um vetor
- Vetorizando é criando os vetores de todos os roteadores, mas será usado só o do roteador selecionado(Reaproveitamento de Código)
- Mostra é a função generica para mostrar a tabela do roteador
- Bellmanford é a função principal, a qual condiz com o trabalho, sendo ela usando outros vetores recebidos para recalcular o seu proprio
- Tabelalamento é a criação das tabela de cada vetor, mas é usado apenas a tabela do roteador selecionado
- Nodos enlace e roteador são funções para criar um novo malloc destes tipos de nodos.
- Leituraroteador é a leitura do arquivo roteador.config para saber os existem, mas é usado apenas os vizinhos do roteador selecionado(Reaproveitamento de Código)
- Arestas é a montagem das arestas dos roteadores
- Leituraenlace leitura do arquivo enlace.config passando a montagem para a função anterior
- Releitura é função que lê o arquivo de roteador.config para montar o full-duplex(2 caminhos) dos roteadores.
- Die, printf das funções udp
- Verifica, apenas vê se existe roteadores digitados
- Client função para envio de mensagens normais de usuário
- Server função para receber mensagens onde são separadas em controle ou usuário
- Envio função que monta mensagem usuário
- Client_verife função de envio de mensagens de controle, ou seja, vetores proprios do roteador
- Atualiza_vector função que em tempos verifica vetores da composição da tabela do roteador
- Verifica_roteador função que inicia threads para receber,enviar e atualizar vetores e mensagens.
 
EXECUÇÃO:
- Execute o arquivo comunicacao.c
- Digite qual roteador iniciar
- Espera a conversão dos roteadores iniciados e/ou desligados
- Digite o roteador destino
- Digite mensagem
- Caso queira ver a tabela do roteador
- Digite -2 e msg
