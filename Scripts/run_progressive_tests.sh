#!/usr/bin/env bash
set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/compilador_racket"
TEST_DIR="$ROOT_DIR/Examples"

if ! command -v make >/dev/null 2>&1; then
  echo "Erro: make não encontrado no PATH." >&2
  exit 1
fi

if ! make -s -C "$ROOT_DIR"; then
  echo "Erro: falha na compilação." >&2
  exit 1
fi

if [[ ! -x "$BIN" ]]; then
  echo "Erro: executável não encontrado em $BIN" >&2
  exit 1
fi

if [[ ! -d "$TEST_DIR" ]]; then
  echo "Erro: diretório de testes progressivos não encontrado em $TEST_DIR" >&2
  exit 1
fi

pass_count=0
fail_count=0

declare -a failures=()

total=$((8 * 3))

evaluate_case() {
  local level="$1"
  local kind="$2"
  local expected="$3"
  local file="$TEST_DIR/L${level}_${kind}.rkt"

  if [[ ! -f "$file" ]]; then
    echo "[FAIL] L${level}_${kind}: arquivo não encontrado"
    failures+=("L${level}_${kind} (arquivo ausente)")
    fail_count=$((fail_count + 1))
    return
  fi

  local output
  output="$($BIN "$file" 2>&1 || true)"

  local ok=0
  case "$expected" in
    accepted)
      if grep -q "Programa aceito\." <<< "$output"; then
        ok=1
      fi
      ;;
    lex_error)
      if grep -q "Erros léxicos" <<< "$output"; then
        ok=1
      fi
      ;;
    syn_error)
      if grep -q "Erros sintáticos" <<< "$output"; then
        ok=1
      fi
      ;;
  esac

  if [[ "$ok" -eq 1 ]]; then
    echo "[PASS] L${level}_${kind}"
    pass_count=$((pass_count + 1))
  else
    echo "[FAIL] L${level}_${kind}"
    echo "------ saída capturada ------"
    echo "$output"
    echo "-----------------------------"
    failures+=("L${level}_${kind}")
    fail_count=$((fail_count + 1))
  fi
}

echo "Executando suíte progressiva (8 níveis x 3 casos)..."
for level in {1..8}; do
  evaluate_case "$level" "valid" "accepted"
  evaluate_case "$level" "invalid_lex" "lex_error"
  evaluate_case "$level" "invalid_syn" "syn_error"
  echo ""
done

echo "Resumo final"
echo "- Total: $total"
echo "- Pass:  $pass_count"
echo "- Fail:  $fail_count"

if [[ "$fail_count" -gt 0 ]]; then
  echo "Falhas:" 
  for item in "${failures[@]}"; do
    echo "- $item"
  done
  exit 1
fi

echo "Suíte progressiva concluída com sucesso."
exit 0
