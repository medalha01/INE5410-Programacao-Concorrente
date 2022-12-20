from dataclasses import dataclass

from utils.currency import Currency
from utils.logger import LOGGER
from threading import Lock

@dataclass
class Account:
    """
    Uma classe para representar uma conta bancária.
    Se você adicionar novos atributos ou métodos, lembre-se de atualizar essa docstring.

    ...

    Atributos
    ---------
    _id: int
        Identificador da conta bancária.
    _bank_id: int
        Identificador do banco no qual a conta bancária foi criada.
    currency : Currency
        Moeda corrente da conta bancária.
    balance : int
        Saldo da conta bancária.
    overdraft_limit : int
        Limite de cheque especial da conta bancária.
    _mutex:
        Mutex específico da conta para garantir que não existirão erros de concorrência no read & write do self.balance

    Métodos
    -------
    info() -> None:
        Printa informações sobre a conta bancária.
    deposit(amount: int) -> None:
        Adiciona o valor `amount` ao saldo da conta bancária.
    withdraw(amount: int) -> None:
        Remove o valor `amount` do saldo da conta bancária.
    """

    _id: int
    _bank_id: int
    currency: Currency
    balance: int = 0
    overdraft_limit: int = 0
    _mutex = Lock()


    def set_balance(self, val: int) -> None:
        self.balance = val


    def set_overdraft_limit(self, val: int) -> None:
        self.overdraft_limit = val


    def info(self) -> None:
        """
        Esse método printa informações gerais sobre a conta bancária.
        """
        # TODO: IMPLEMENTE AS MODIFICAÇÕES, SE NECESSÁRIAS, NESTE MÉTODO!

        pretty_balance = f"{format(round(self.balance/100), ',d')}.{self.balance%100:02d} {self.currency.name}"
        pretty_overdraft_limit = f"{format(round(self.overdraft_limit/100), ',d')}.{self.overdraft_limit%100:02d} {self.currency.name}"
        LOGGER.info(f"Account::{{ _id={self._id}, _bank_id={self._bank_id}, balance={pretty_balance}, overdraft_limit={pretty_overdraft_limit} }}")


    def deposit(self, amount: int) -> bool:
        """
        Esse método deverá adicionar o valor `amount` passado como argumento ao saldo da conta bancária 
        (`balance`). Lembre-se que esse método pode ser chamado concorrentemente por múltiplos 
        PaymentProcessors, então modifique-o para garantir que não ocorram erros de concorrência!
        """
        # TODO: IMPLEMENTE AS MODIFICAÇÕES NECESSÁRIAS NESTE MÉTODO !

        with self._mutex:
            self.balance += amount
        LOGGER.info(f"deposit({amount}) successful!")
        return True


    def withdraw(self, amount: int) -> tuple:
        """
        Esse método deverá retirar o valor `amount` especificado do saldo da conta bancária (`balance`).
        Deverá ser retornado um valor bool indicando se foi possível ou não realizar a retirada.
        Lembre-se que esse método pode ser chamado concorrentemente por múltiplos PaymentProcessors, 
        então modifique-o para garantir que não ocorram erros de concorrência!
        """
        # TODO: IMPLEMENTE AS MODIFICAÇÕES NECESSÁRIAS NESTE MÉTODO !

        # mutex da conta garante o acesso sem erros de concorrência
        # retorna uma tupla que indica se a transação foi completada corretamente [2] e
        # se usou o overdraft [0, com o valor do mesmo em [1]
        with self._mutex:
            if self.balance >= amount:
                self.balance -= amount
                LOGGER.info(f"withdraw({amount}) successful!")
                return (False, 0, True)
            else:
                overdrafted_amount = abs(self.balance - amount)
                if self.overdraft_limit >= overdrafted_amount:
                    self.balance -= amount
                    LOGGER.info(f"withdraw({amount}) successful with overdraft!")
                    return (True, overdrafted_amount, True)
        LOGGER.warning(f"withdraw({amount}) failed, no balance!")
        return (False, 0, False)


@dataclass
class CurrencyReserves:
    """
    Uma classe de dados para armazenar as reservas do banco, que serão usadas
    para câmbio e transferências internacionais.
    OBS: NÃO É PERMITIDO ALTERAR ESSA CLASSE!
    """

    USD: Account = Account(_id=1, _bank_id=0, currency=Currency.USD)
    EUR: Account = Account(_id=2, _bank_id=0, currency=Currency.EUR)
    GBP: Account = Account(_id=3, _bank_id=0, currency=Currency.GBP)
    JPY: Account = Account(_id=4, _bank_id=0, currency=Currency.JPY)
    CHF: Account = Account(_id=5, _bank_id=0, currency=Currency.CHF)
    BRL: Account = Account(_id=6, _bank_id=0, currency=Currency.BRL)
