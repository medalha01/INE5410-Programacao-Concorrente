from time import sleep
from random import randint
from threading import Thread, Semaphore

def produtor():
  global buffer
  for i in range(10):
    sleep(randint(0,2))           # fica um tempo produzindo...
    item = 'item ' + str(i)
    # verifica se há lugar no buffer
    # mutex.acquire()
    empty.acquire()
    buffer.append(item)
    print('Produzido %s (ha %i itens no buffer)' % (item,len(buffer)))
    full.release()
    # mutex.release()

def consumidor():
  global buffer
  for i in range(10):
    # aguarda que haja um item para consumir 
    # mutex.acquire()
    full.acquire()
    item = buffer.pop(0)
    print('Consumido %s (ha %i itens no buffer)' % (item,len(buffer)))
    sleep(randint(0,2))         # fica um tempo consumindo...
    empty.release()
    # mutex.release()

buffer = []
tam_buffer = 3
# cria semáforos
empty = Semaphore(tam_buffer)
full = Semaphore(0)
# mutex = Semaphore()
produtor = Thread(target=produtor) 
consumidor = Thread(target=consumidor) 
produtor.start()
consumidor.start()
produtor.join()
consumidor.join() 