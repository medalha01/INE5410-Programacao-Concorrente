from dataclasses import dataclass
from datetime import datetime, timedelta
from enum import Enum
from typing import Optional, Tuple

from utils.currency import Currency
from utils.logger import LOGGER


class TransactionStatus(Enum):
    """
    Uma Enum para os diferentes status das transações bancárias.
    OBS: NÃO É PERMITIDO ALTERAR AS VARIANTES DESSA ENUM!
    """
    PENDING = 1
    FAILED = 2
    SUCCESSFUL = 3


@dataclass
class Transaction:
    """
    Uma dataclass para representar uma transação bancária.
    OBS: NÃO É PERMITIDO ALTERAR OS ATRIBUTOS NEM OS MÉTODOS DESTA CLASSE!
    
    ...

    Atributos
    ---------
    _id : int
        Um identificador da transação bancária.
    origin : Tuple[int, int]
        Uma tupla contendo o identificador do banco e da conta de origem.
    destination : Tuple[int, int]
        Uma tupla contendo o identificador do banco e da conta destino.
    amount : int
        Valor a ser transferido.
    currency : Currency
        Moeda a ser transferida.
    status : TransactionStatus
        Status da transação bancária.
    created_at : datetime
        Timestamp do momento de criação da transação bancária (quando ela é requisitada pelo cliente).
    completed_at : datetime
        Timestamp do momento em que a transação é finalizada (seja status FAILED ou SUCCESSFUL).

    Métodos
    -------
    set_status(status: TransactionStatus) -> None:
        Define um novo status para a transação bancária.
    """

    _id: int
    origin: Tuple[int, int]
    destination: Tuple[int, int] 
    amount: int
    currency: Currency
    exchange_fee: int = 0
    taxes: int = 0
    status: TransactionStatus = TransactionStatus.PENDING
    created_at: datetime = datetime.now()
    completed_at: Optional[datetime] = None

    def set_status(self, status: TransactionStatus) -> None:
        """
        Define um novo status para a transação bancária.
        Essa função deve atualizar self.status e self.completed.
        ATENÇÃO: NÃO É PERMITIDO ALTERAR ESSE MÉTODO!
        """
        self.status = status
        self.completed_at = datetime.now()


    def get_processing_time(self) -> Optional[timedelta]:
        """
        Retorna o intervalo de tempo necesário para o processamento completo da transação.
        Essa função deve retornar None caso a transação ainda não tenha finalizado.
        ATENÇÃO: NÃO É PERMITIDO ALTERAR ESSE MÉTODO!
        """
        if self.completed_at != None:
            return self.completed_at - self.created_at
        else:
            LOGGER.warn(f"Attempt to get processing time of pending transaction!")
            return None
