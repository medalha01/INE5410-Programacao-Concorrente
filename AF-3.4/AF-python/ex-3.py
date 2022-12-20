from multiprocessing import Process, Queue

def origem(fila1):
  for i in range(10):
    msg = 'Teste ' + str(i)
    fila1.put(msg)          # envia mensagem para a ponte pela fila 1
    print('Mensagem enviada:', msg)

def ponte(fila1, fila2):
  for i in range(10):
    message = fila1.get()             # recebe mensagem na fila 1
    fila2.put(message)                # reenvia mensagem na fila 2
    print('Mensagem encaminhada:', message)

def destino(fila2):
  for i in range(10):
    message = fila2.get()                # recebe mensagem na fila 2
    print('Mensagem recebida:', message)

if __name__ == '__main__':
  fila1 = Queue()
  fila2 = Queue()
  o = Process(target=origem, args=(fila1,))
  p = Process(target=ponte, args=(fila1,fila2))
  d = Process(target=destino, args=(fila2,))
  # executa os processos
  o.start()
  p.start()
  d.start()
  # aguarda conclusão
  o.join()
  p.join()
  d.join()
  # libera as filas
  fila1.close()
  fila2.close()