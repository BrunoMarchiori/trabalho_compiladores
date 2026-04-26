# Matriz de Testes Progressivos

| Nível | Objetivo | Features cobertas | Arquivos | Resultado esperado |
|---|---|---|---|---|
| L1 | Base mínima de parsing | `define`, chamada simples, número, símbolo | `L1_valid.rkt`, `L1_invalid_lex.rkt`, `L1_invalid_syn.rkt` | válido aceita; inválido léxico rejeita por léxico; inválido sintático rejeita por sintaxe |
| L2 | Fluxo condicional básico | `if`, operadores aritméticos e relacionais | `L2_valid.rkt`, `L2_invalid_lex.rkt`, `L2_invalid_syn.rkt` | idem |
| L3 | Abstração e aninhamento | `lambda`, chamada aninhada | `L3_valid.rkt`, `L3_invalid_lex.rkt`, `L3_invalid_syn.rkt` | idem |
| L4 | Condicional multi-cláusula | `cond`, cláusulas com `()` e `[]`, `else` | `L4_valid.rkt`, `L4_invalid_lex.rkt`, `L4_invalid_syn.rkt` | idem |
| L5 | Escopo e bindings | `let`, `let*`, `letrec`, bindings com `()` e `[]` | `L5_valid.rkt`, `L5_invalid_lex.rkt`, `L5_invalid_syn.rkt` | idem |
| L6 | Citação e dados literais | `'`, `` ` ``, `,`, `'()` | `L6_valid.rkt`, `L6_invalid_lex.rkt`, `L6_invalid_syn.rkt` | idem |
| L7 | Construções Racket reais | `#lang`, `struct`, `#:atributo`, `for`, `for/list`, `match`, `module+` | `L7_valid.rkt`, `L7_invalid_lex.rkt`, `L7_invalid_syn.rkt` | idem |
| L8 | Cenário composto end-to-end | múltiplas formas em um programa maior | `L8_valid.rkt`, `L8_invalid_lex.rkt`, `L8_invalid_syn.rkt` | idem |

Diretório dos casos: `Examples/`.

## Execução

```bash
bash Scripts/run_progressive_tests.sh
```

## Critérios globais

- 24 casos executados (8 níveis x 3 casos)
- caso válido: saída contém `Programa aceito.`
- caso inválido léxico: saída contém `Erros léxicos`
- caso inválido sintático: saída contém `Erros sintáticos`
