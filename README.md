# Exemplo de programas com Memória Compartilhada

## Montando Writer
```
gcc writer.c -o writer -lm -lrt -pthread
```

## Montando Reader
```
gcc reader.c -o reader -lm -lrt -pthread
```

## Executando Writer
```
./writer
```

## executando Reader
```
./reader
```
