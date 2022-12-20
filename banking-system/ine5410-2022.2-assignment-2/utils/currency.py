from enum import IntEnum

from globals import *


class Currency(IntEnum):
    """
    Uma Enum para os diferentes tipos de moedas correntes disponíveis.
    OBS: NÃO É PERMITIDO ALTERAR AS VARIANTES DESSA ENUM!
    """
    USD = 1
    EUR = 2
    GBP = 3
    JPY = 4
    CHF = 5
    BRL = 6


def get_exchange_rate(f: Currency, t: Currency) -> float:
    if f == Currency.USD:
        if t == Currency.USD:
            return 1
        elif t == Currency.EUR:
            return 0.98
        elif t == Currency.GBP:
            return 0.85
        elif t == Currency.JPY:
            return 141.94
        elif t == Currency.CHF:
            return 0.98
        elif t == Currency.BRL:
            return 5.35
    elif f == Currency.EUR:
        if t == Currency.USD:
            return 1.02
        elif t == Currency.EUR:
            return 1
        elif t == Currency.GBP:
            return 0.87
        elif t == Currency.JPY:
            return 145.46
        elif t == Currency.CHF:
            return 0.98
        elif t == Currency.BRL:
            return 5.47
    elif f == Currency.GBP:
        if t == Currency.USD:
            return 1.18
        elif t == Currency.EUR:
            return 1.15
        elif t == Currency.GBP:
            return 1
        elif t == Currency.JPY:
            return 167.41
        elif t == Currency.CHF:
            return 1.13
        elif t == Currency.BRL:
            return 6.30
    elif f == Currency.JPY:
        if t == Currency.USD:
            return 0.0070
        elif t == Currency.EUR:
            return 0.0069
        elif t == Currency.GBP:
            return 0.0060
        elif t == Currency.JPY:
            return 1
        elif t == Currency.CHF:
            return 0.0068
        elif t == Currency.BRL:
            return 0.038
    elif f == Currency.CHF:
        if t == Currency.USD:
            return 1.04
        elif t == Currency.EUR:
            return 1.02
        elif t == Currency.GBP:
            return 0.89
        elif t == Currency.JPY:
            return 148.07
        elif t == Currency.CHF:
            return 1
        elif t == Currency.BRL:
            return 5.57
    else:
        if t == Currency.USD:
            return 0.19
        elif t == Currency.EUR:
            return 0.18
        elif t == Currency.GBP:
            return 0.16
        elif t == Currency.JPY:
            return 26.59
        elif t == Currency.CHF:
            return 0.18
        elif t == Currency.BRL:
            return 1
