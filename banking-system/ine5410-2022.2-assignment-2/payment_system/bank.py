from typing import Tuple
from threading import Lock
import random as rd
from queue import Queue

from globals import *
from payment_system.account import Account, CurrencyReserves
from utils.transaction import Transaction
from utils.currency import Currency
from utils.logger import LOGGER

class Bank():
    """
    Uma classe para representar um Banco.
    Se você adicionar novos atributos ou métodos, lembre-se de atualizar essa docstring.

    ...

    Atributos
    ---------
    _id : int
        Identificador do banco.
    currency : Currency
        Moeda corrente das contas bancárias do banco.
    reserves : CurrencyReserves
        Dataclass de contas bancárias contendo as reservas internas do banco.
    operating : bool
        Booleano que indica se o banco está em funcionamento ou não.
    accounts : List[Account]
        Lista contendo as contas bancárias dos clientes do banco.
    transaction_queue : Queue[Transaction]
        Fila FIFO contendo as transações bancárias pendentes que ainda serão processadas.
    q_sem: Semaphore
        Semáforo utilizado para sincronizar o acesso às transações, garantindo o uso correto.
    q_lock: Lock
        Mutex para garantir acesso atômico à queue de transações.
    n_nat_transactions: int
        Conta o número de transações nacionais.
    n_nat_mutex: Lock
        Mutex que garante o incremento atômico do contador acima.
    n_int_transactions: int
        Conta o número de transações internacionais.
    n_int_mutex: Lock
        Mutex que garante o incremento atômico do contador acima.

    Métodos
    -------
    new_account(balance: int = 0, overdraft_limit: int = 0) -> None:
        Cria uma nova conta bancária (Account) no banco.
    new_transfer(origin: Tuple[int, int], destination: Tuple[int, int], amount: int, currency: Currency) -> None:
        Cria uma nova transação bancária.
    info() -> None:
        Printa informações e estatísticas sobre o funcionamento do banco.
    
    """

    def __init__(self, _id: int, currency: Currency):
        self._id                = _id
        self.currency           = currency
        self.reserves           = CurrencyReserves()
        self.operating          = True                               # começa a operar quando é criado
        self.transaction_queue  = Queue() 
        self.q_sem              = Semaphore(0)
        self.q_mutex            = Lock()
        self.n_nat_transactions = 0
        self.n_nat_mutex        = Lock()
        self.n_int_transactions = 0
        self.n_int_mutex        = Lock()
        self.accounts           = self.__create_random_accounts(101) # randint(0, 100) inclui tanto o 0 qnto o 100, são 101 contas
        self.profit             = 0
        self.profit_mutex       = Lock()


    def __create_random_accounts(self, n_accs: int) -> list:
        """
        Esse método cria n_accs contas com fundos aleatórios e as retorna em forma de lista.

        Decidimos usar esse método aqui por questão de conveniência ao invés do que já estava "pronto".

        @param n_accs: número inteiro representando o número de contas a serem criadas.
        @return list: lista de contas criadas.
        """
        accounts = list()
        for i in range(n_accs):
            new_acc: Account = Account(_id=i+1, _bank_id=self._id, currency=self.currency)
            new_acc.set_balance(rd.randint(1_000_000, 90_000_000_000))
            new_acc.set_overdraft_limit(rd.randint(0, 5_000))
            accounts.append(new_acc)
        return accounts          


    def info(self) -> None:
        """
        Essa função deverá printar os seguintes dados utilizando o LOGGER fornecido:
        1. done --- Saldo de cada moeda nas reservas internas do banco
        2. done --- Número de transferências nacionais e internacionais realizadas
        3. done --- Número de contas bancárias registradas no banco
        4. done --- Saldo total de todas as contas bancárias (dos clientes) registradas no banco
        5. Lucro do banco: taxas de câmbio acumuladas + juros de cheque especial acumulados
        """
        # TODO: IMPLEMENTE AS MODIFICAÇÕES, SE NECESSÁRIAS, NESTE MÉTODO!

        LOGGER.info(f"/¨¨¨ Estatísticas do Banco Nacional {self._id}: ¨¨¨¨¨¨¨¨¨¨¨¨¨")
        LOGGER.info("|  {!} Saldo de cada moeda nas reservas internas")
        LOGGER.info(f"|  --> BRL: {self.reserves.BRL.balance}")
        LOGGER.info(f"|  --> USD: {self.reserves.USD.balance}")
        LOGGER.info(f"|  --> EUR: {self.reserves.EUR.balance}")
        LOGGER.info(f"|  --> JPY: {self.reserves.JPY.balance}")
        LOGGER.info(f"|  --> CHF: {self.reserves.CHF.balance}")
        LOGGER.info(f"|  --> GBṔ: {self.reserves.GBP.balance}")
        LOGGER.info("|  {!} Número de transferências nacionais e internacionis realizadas")
        LOGGER.info(f"|  --> Nacionais     : {self.n_nat_transactions}")
        LOGGER.info(f"|  --> Internacionais: {self.n_int_transactions}")
        LOGGER.info("|  {!} Saldo total de todas as contas dos clientes do banco")
        total_balance = 0
        for acc in self.accounts:
            total_balance += acc.balance
        LOGGER.info(f"|  --> {total_balance}")
        LOGGER.info("|  {!} Número de contas bancárias registradas no banco")
        LOGGER.info("|  --> " + str(len(self.accounts)))
        LOGGER.info("|  {!} Lucro do banco (taxas de câmbio acumuladas + juros de cheque especial acumulados")
        LOGGER.info(f"|  --> {self.profit}")
        LOGGER.info("\___________________________________________________\n")