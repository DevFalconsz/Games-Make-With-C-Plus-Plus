@echo off
REM Este é um arquivo de lote para compilar e construir o programa
g++ -IC:\SFML-2.5.1\include -c main.cpp -o main.o
g++ -LC:\SFML-2.5.1\lib .\main.o -o main.exe -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-main -mwindows