from threading import Semaphore

# Lista de Bancos Nacionais
banks = []

# Printar Logs de DEBUG no console?
debug = False

# Tempo total de simulação
total_time = 1000

# Unidade de tempo (quanto menor, mais rápida a simulação)
time_unit = 0.005  # 0.1 = 100ms

"""COISAS QUE ADICIONAMOS ABAIXO"""
