import time
from threading import Thread
from queue import Queue

from globals import *
from payment_system.bank import Bank
from utils.currency import *
from utils.transaction import Transaction, TransactionStatus
from utils.logger import LOGGER


class PaymentProcessor(Thread):
    """
    Uma classe para representar um processador de pagamentos de um banco.
    Se você adicionar novos atributos ou métodos, lembre-se de atualizar essa docstring.

    ...

    Atributos
    ---------
    _id : int
        Identificador do processador de pagamentos.
    bank: Bank
        Banco sob o qual o processador de pagamentos operará.

    Métodos
    -------
    run():
        Inicia thread to PaymentProcessor
    process_transaction(transaction: Transaction) -> TransactionStatus:
        Processa uma transação bancária.
    """

    def __init__(self, _id: int, bank: Bank):
        Thread.__init__(self)
        self._id  = _id
        self.bank = bank


    def run(self) -> None:
        """
        Esse método deve buscar Transactions na fila de transações do banco e processá-las 
        utilizando o método self.process_transaction(self, transaction: Transaction).
        Ele não deve ser finalizado prematuramente (antes do banco realmente fechar).
        """
        # TODO: IMPLEMENTE/MODIFIQUE O CÓDIGO NECESSÁRIO ABAIXO !

        LOGGER.info(f"Inicializado o PaymentProcessor {self._id} do Banco {self.bank._id}!")
        queue: Queue = banks[self.bank._id].transaction_queue
        
        # adquire um semáforo referente ao banco questão e não permite a propagação de erros no método pop
        # aqui nem precisa de um try ... except devido ao semáforo
        while self.bank.operating:
            self.bank.q_sem.acquire()
            
            with self.bank.q_mutex:   
                transaction = queue.get()
            
            self.process_transaction(transaction)
        LOGGER.info(f"O PaymentProcessor {self._id} do banco {self.bank._id} foi finalizado.")


    def __international_transaction(self, transaction: Transaction,
                                    intermediary_deposit: float, final_deposit: float) -> TransactionStatus:
        """
        Esse método processa uma transação internacional, ou seja, tem a taxa de 1% aplicada e é feita
        entre bancos diferentes.

        @param transaction:             a dataclass de transição a ser feita
        @param intermediary_deposit:    o depósito a ser feito para a conta intermediária especial
        @param final_deposit            o depósito a ser feito ao recipiente final

        @return TransactionStatus
        """

        # TODO: MUTEXES E TALS
        if transaction.currency == Currency.EUR:
            self.bank.reserves.EUR.deposit(intermediary_deposit)
            self.bank.reserves.EUR.withdraw(final_deposit)
        elif transaction.currency == Currency.USD:
            self.bank.reserves.USD.deposit(intermediary_deposit)
            self.bank.reserves.USD.withdraw(final_deposit)
        elif transaction.currency == Currency.CHF:
            self.bank.reserves.CHF.deposit(intermediary_deposit)
            self.bank.reserves.CHF.withdraw(final_deposit)
        elif transaction.currency == Currency.JPY:
            self.bank.reserves.JPY.deposit(intermediary_deposit)
            self.bank.reserves.JPY.withdraw(final_deposit)
        elif transaction.currency == Currency.GBP:
            self.bank.reserves.GBP.deposit(intermediary_deposit)
            self.bank.reserves.GBP.withdraw(final_deposit)
        else:
            transaction.set_status(TransactionStatus.FAILED)
            return transaction.status

        dest_acc = banks[transaction.destination[0]].accounts[transaction.destination[1]]
        dest_acc.deposit(final_deposit)

        # Atualizando o contador de transações internacionais, protegido por mutex
        with self.bank.n_int_mutex:
            self.bank.n_int_transactions += 1

        transaction.set_status(TransactionStatus.SUCCESSFUL)
        return transaction.status


    def __national_transaction(self, transaction: Transaction,
                                intermediary_deposit: float, final_deposit: float) -> TransactionStatus:
        """
        Esse método processa uma transação nacional, ou seja, não tem a taxa de 1% aplicada e é feita
        dentro do mesmo banco de origem.

        @param transaction:             a dataclass de transição a ser feita
        @param intermediary_deposit:    o depósito a ser feito para a conta intermediária especial
        @param final_deposit            o depósito a ser feito ao recipiente final

        @return TransactionStatus
        """

        self.bank.reserves.BRL.deposit(intermediary_deposit)
        self.bank.reserves.BRL.withdraw(final_deposit)
        
        dest_acc = banks[transaction.destination[0]].accounts[transaction.destination[1]]
        dest_acc.deposit(final_deposit)

        # Atualizando o contador de transações nacionais do banco, protegido por mutex
        with self.bank.n_nat_mutex:
            self.bank.n_nat_transactions += 1

        transaction.set_status(TransactionStatus.SUCCESSFUL)
        return transaction.status


    def process_transaction(self, transaction: Transaction) -> TransactionStatus:
        """
        Esse método deverá processar as transações bancárias do banco ao qual foi designado.
        Caso a transferência seja realizada para um banco diferente (em moeda diferente), a 
        lógica para transações internacionais detalhada no enunciado (README.md) deverá ser
        aplicada.
        Ela deve retornar o status da transacão processada.
        """
        # TODO: IMPLEMENTE/MODIFIQUE O CÓDIGO NECESSÁRIO ABAIXO !

        LOGGER.info(f"PaymentProcessor {self._id} do Banco {self.bank._id} iniciando processamento da Transaction {transaction._id}!")
        
        # Pegando a conta de origem 
        og_acc = banks[transaction.origin[0]].accounts[transaction.origin[1]]

        # Pegando a taxa de conversão
        rate = get_exchange_rate(self.bank.currency, transaction.currency)

        # Withdrawing da conta origem e retirando informações relevantes
        (has_overdraft, value_overdraft, successful) = og_acc.withdraw(transaction.amount)

        # Realizando withdraw da conta origem para tirar o dinheiro e passando para a reserva interna
        # que é a conta especial. Após isso, é retirado dessa conta intermediária para transferir para o destino 
        # TODO: semáforo acquire das contas aqui (LIST INDEX OUT OF RANGE)
        if successful:
            rate_applied = transaction.amount * rate
            
            # Applying fees 
            if not has_overdraft:
                withdraw_international = rate_applied * 0.99
                withdraw_national = rate_applied 
                with self.bank.profit_mutex:
                    self.bank.profit += rate_applied * 0.01
            else:
                withdraw_international = (value_overdraft * 0.95 + transaction.amount - value_overdraft) * rate * 0.99
                withdraw_national = (value_overdraft * 0.95 + transaction.amount - value_overdraft) * rate
                with self.bank.profit_mutex:
                    self.bank.profit += value_overdraft * 0.05
                    self.bank.profit += (value_overdraft * 0.95 + transaction.amount - value_overdraft) * rate * 0.01

            # Checando se a transação é internacional ou nacional
            if transaction.origin[0] != transaction.destination[0]:
                return self.__international_transaction(transaction, rate_applied, withdraw_international)
            else:
                return self.__national_transaction(transaction, rate_applied, withdraw_national)
        else:
            transaction.set_status(TransactionStatus.FAILED)
            return transaction.status
        
