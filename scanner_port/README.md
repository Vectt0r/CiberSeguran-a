Passo a passo para rodar seu scanner TCP no Windows (com scanme.nmap.org)
1. Prepare o ambiente
Instale o MSYS2 (https://www.msys2.org/) para ter um ambiente Unix-like com gcc e OpenSSL.

Abra o terminal MSYS2 MinGW 64-bit.

2. Instale as dependências
No terminal MSYS2, rode:

bash
Copiar
Editar
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-openssl
Isso instala o compilador GCC e a biblioteca OpenSSL.

3. Prepare o código fonte
Coloque seu código scanner.c na pasta C:/Users/Usuario/Desktop/scanner (ou outra que preferir).

No terminal MSYS2, navegue para essa pasta:

bash
Copiar
Editar
cd /c/Users/Usuario/Desktop/scanner
4. Compile o código
Execute o comando para compilar seu scanner com as bibliotecas necessárias:

bash
Copiar
Editar
gcc scanner.c -o scanner.exe -lws2_32 -lssl -lcrypto -lpthread
-lws2_32 = biblioteca Windows Sockets

-lssl e -lcrypto = OpenSSL

-lpthread = threads POSIX (geralmente funcionam no MSYS2)

5. Execute o scanner
Para escanear a faixa de portas 20 a 30 no domínio scanme.nmap.org, rode:

bash
Copiar
Editar
./scanner.exe scanme.nmap.org 20 30
Você verá o resultado no console e os arquivos:

saida.txt

saida.json

saida.csv

criadas na pasta, com os resultados detalhados.

6. Interpretar resultados
Portas com "aberta" são acessíveis.

Você verá banners (respostas dos serviços) e, se aplicável, dados do certificado SSL.

Portas fechadas ou inacessíveis aparecem como "fechada".
