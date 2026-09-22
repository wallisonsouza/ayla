# Ideia: Enum na Ayla

## Objetivo

Adicionar `enum` à Ayla como uma construção para representar um conjunto
fechado de variantes.

A ideia é manter `enum` simples e separado de `struct`:

-   `struct` representa dados compostos.
-   `enum` representa uma escolha entre alternativas.
-   Uma variante pode carregar zero ou mais tipos posicionais.
-   Dados complexos e nomeados devem ser representados por `struct` e
    usados como payload da variante.

## Sintaxe básica

Enum sem payload:

``` ayla
Color: enum {
    Red
    Green
    Blue
}
```

Uso:

``` ayla
color: Color = Color::Red
```

Enum com payload:

``` ayla
Shape: enum {
    Circle(F32)
    Rectangle(F32, F32)
}
```

Uso:

``` ayla
shape: Shape = Shape::Circle(10.0)
shape: Shape = Shape::Rectangle(20.0, 30.0)
```

## Estruturas como payload

Não é necessário permitir campos nomeados diretamente dentro das
variantes.

Em vez de:

``` ayla
Shape: enum {
    RoundedRectangle {
        width: F32
        height: F32
        radius: F32
    }
}
```

usar:

``` ayla
RoundedRectangle: struct {
    width: F32
    height: F32
    radius: F32
}

Shape: enum {
    RoundedRectangle(RoundedRectangle)
}
```

Isso mantém cada construção com uma responsabilidade clara:

``` text
struct → dados
enum   → alternativas
```

Também permite reutilizar `RoundedRectangle` fora de `Shape`.

## Exemplo: Result

Um dos principais usos será representar resultados com sucesso ou erro:

``` ayla
Result: enum<T, E> {
    Success(T)
    Err(E)
}
```

Uso:

``` ayla
result: Result<Int, String> = Result::Success(42)

error: Result<Int, String> = Result::Err("failed")
```

`Result` não precisa de uma construção especial. É apenas um `enum`
genérico.

## Exemplo: Option

``` ayla
Option: enum<T> {
    Some(T)
    None
}
```

Uso:

``` ayla
value: Option<Int> = Option::Some(42)

empty: Option<Int> = Option::None
```

## Exemplo: Token

`enum` também pode ser útil para representar tokens do compilador:

``` ayla
Token: enum {
    Identifier(String)
    Number(Int)
    String(String)
    Plus
    Minus
    EOF
}
```

Assim, cada token é uma variante de `Token` e algumas variantes carregam
dados enquanto outras não.

## Pattern matching

O uso natural de `enum` exige uma forma de verificar a variante e
extrair seu payload.

A ideia é usar `match`:

``` ayla
match shape {
    Shape::Circle(radius) => {
        ...
    }

    Shape::Rectangle(width, height) => {
        ...
    }
}
```

Para enum sem payload:

``` ayla
match color {
    Color::Red => {
        ...
    }

    Color::Green => {
        ...
    }

    Color::Blue => {
        ...
    }
}
```

A sintaxe e as regras completas de `match` ainda precisam ser definidas.

## Regras conceituais

### 1. Enum é um tipo próprio

``` ayla
Color: enum {
    Red
    Green
    Blue
}
```

`Color` é um tipo distinto.

`Color::Red` é um valor desse tipo.

Não deve ser tratado simplesmente como um `Int`, mesmo que a
representação interna use um discriminante inteiro.

### 2. Variantes são fechadas

Depois da declaração:

``` ayla
Color: enum {
    Red
    Green
    Blue
}
```

não é possível adicionar outra variante externamente.

### 3. Variantes podem ter zero ou mais payloads

``` ayla
A: enum {
    None
    One(Int)
    Two(Int, String)
}
```

### 4. Payloads são posicionais

A princípio, não permitir:

``` ayla
A: enum {
    Value {
        x: Int
        y: Int
    }
}
```

Para dados nomeados, usar `struct`:

``` ayla
ValueData: struct {
    x: Int
    y: Int
}

A: enum {
    Value(ValueData)
}
```

### 5. Enum pode ser genérico

``` ayla
Result: enum<T, E> {
    Success(T)
    Err(E)
}
```

Isso deve usar o mesmo sistema de parâmetros genéricos das demais
declarações:

``` ayla
Array: struct<T> {
    ...
}

Result: enum<T, E> {
    ...
}
```

## AST proposta

Uma possível representação:

``` text
EnumDeclaration
├── Name
├── GenericParameters
└── Variants
    ├── EnumVariant
    │   ├── Name
    │   └── PayloadTypes
    └── ...
```

Exemplo:

``` ayla
Result: enum<T, E> {
    Success(T)
    Err(E)
}
```

AST conceitual:

``` text
EnumDeclaration
├── Name
│   └── Identifier("Result")
├── GenericParameters
│   ├── T
│   └── E
└── Variants
    ├── EnumVariant
    │   ├── Name: Success
    │   └── PayloadTypes
    │       └── NamedType(T)
    └── EnumVariant
        ├── Name: Err
        └── PayloadTypes
            └── NamedType(E)
```

## Representação no tipo

Um `enum` deve ser um tipo próprio, com um conjunto conhecido de
variantes.

Conceitualmente:

``` text
EnumType
├── name
├── generic parameters
└── variants
    ├── name
    └── payload types
```

Uma instanciação genérica:

``` ayla
Result<Int, String>
```

deve produzir uma instância do tipo `Result` com:

``` text
Success(Int)
Err(String)
```

## Representação no backend

A representação concreta no C ainda precisa ser definida.

Uma possibilidade é um tagged union:

``` c
typedef struct {
    enum {
        RESULT_SUCCESS,
        RESULT_ERR
    } tag;

    union {
        int success;
        const char *err;
    } data;
} Result;
```

Isso é apenas uma ideia inicial. A representação final deve considerar:

-   tamanho e alinhamento;
-   tipos não triviais;
-   ownership;
-   referências;
-   generics;
-   destruição/cópia, caso existam;
-   layout ABI.

## Relação com Rust

O conceito é semelhante ao `enum` do Rust:

``` rust
enum Shape {
    Circle(f32),
    Rectangle(f32, f32),
}
```
``` ayla
Shape: enum {
    Circle(f32),
    Rectangle(f32, f32),
}
```

e:

``` rust
Result: enum<T, E> {
    Ok(T),
    Err(E),
}
```

Porém, a Ayla não precisa necessariamente copiar todas as
características do Rust.

A proposta atual é deliberadamente mais simples:

``` text
Ayla enum
→ variantes
→ payloads posicionais
→ sem struct fields dentro da variante
```

## Fora do escopo inicial

A implementação de `enum` não precisa acontecer imediatamente.

Antes dela, devem estar suficientemente estáveis:

-   `struct`;
-   tipos;
-   generics;
-   símbolos;
-   resolução;
-   type checking;
-   `impl`/capabilities.

Quando `enum` for implementado, `match` provavelmente deverá ser
desenvolvido junto ou logo depois, pois é o principal mecanismo para
consumir valores de enum.

## Decisões ainda abertas

-   Nome definitivo das variantes e convenções de acesso (`Color::Red`
    etc.).
-   Se o discriminante será observável pelo programa.
-   Se será permitido converter enum para inteiro.
-   Regras de igualdade entre enums.
-   Regras de construção das variantes.
-   Pattern matching completo.
-   Exhaustiveness checking.
-   Representação de enums genéricos no backend.
-   Layout e ABI.
-   Ownership/movimentação dos payloads.
-   Se haverá variantes recursivas diretamente ou por referência.
