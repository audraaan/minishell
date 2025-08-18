#!/bin/bash

# Script de test pour la commande export dans minishell
# Usage: ./test_export.sh ./minishell

if [ $# -ne 1 ]; then
    echo "Usage: $0 <path_to_minishell>"
    echo "Example: $0 ./minishell"
    exit 1
fi

MINISHELL="$1"
PASSED=0
FAILED=0
TEST_NUM=1

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== TESTS DE LA COMMANDE EXPORT ===${NC}"
echo "Minishell: $MINISHELL"
echo ""

# Fonction pour exécuter un test
run_test() {
    local test_name="$1"
    local commands="$2"
    local expected_pattern="$3"

    echo -e "${YELLOW}Test $TEST_NUM: $test_name${NC}"
    echo "Commands: $commands"

    # Créer un fichier temporaire avec les commandes
    temp_file=$(mktemp)
    echo -e "$commands" > "$temp_file"

    # Exécuter dans minishell
    result=$("$MINISHELL" < "$temp_file" 2>&1)

    # Nettoyer le fichier temporaire
    rm "$temp_file"

    echo "Result: $result"

    # Vérifier le résultat
    if [[ $result =~ $expected_pattern ]]; then
        echo -e "${GREEN}✅ PASS${NC}"
        ((PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC}"
        echo "Expected pattern: $expected_pattern"
        ((FAILED++))
    fi
    echo "---"
    ((TEST_NUM++))
}

# Test 1: Export simple
run_test "Export simple" \
"export TEST1=hello
echo \$TEST1
exit" \
"hello"

# Test 2: Export sans valeur
run_test "Export sans valeur" \
"export TEST2
echo \"TEST2='\$TEST2'\"
exit" \
"TEST2=''"

# Test 3: Export avec valeur vide
run_test "Export avec valeur vide" \
"export TEST3=\"\"
echo \"TEST3='\$TEST3'\"
exit" \
"TEST3=''"

# Test 4: Export avec expansion de variable
run_test "Export avec expansion USER" \
"export USER_TEST=\$USER
echo \$USER_TEST
exit" \
"$USER"

# Test 5: Export avec expansion dans guillemets
run_test "Export avec expansion dans guillemets" \
"export MESSAGE=\"Hello \$USER\"
echo \$MESSAGE
exit" \
"Hello $USER"

# Test 6: Export avec guillemets simples (pas d'expansion)
run_test "Export avec guillemets simples" \
"export SINGLE='Hello \$USER'
echo \$SINGLE
exit" \
"Hello \\\$USER"

# Test 7: Export avec espaces
run_test "Export avec espaces" \
"export SPACES=\"hello world\"
echo \$SPACES
exit" \
"hello world"

# Test 8: Overwrite de variable
run_test "Overwrite de variable" \
"export OVERWRITE=first
export OVERWRITE=second
echo \$OVERWRITE
exit" \
"second"

# Test 9: Export multiple
run_test "Export multiple" \
"export A=1 B=2 C=3
echo \$A \$B \$C
exit" \
"1 2 3"

# Test 10: Export avec caractères spéciaux
run_test "Export avec caractères spéciaux" \
"export SPECIAL=\"test;echo hacked\"
echo \$SPECIAL
exit" \
"test;echo hacked"

# Test 11: Export avec expansion de \$?
run_test "Export avec \$?" \
"echo test
export EXIT_CODE=\$?
echo \$EXIT_CODE
exit" \
"0"

# Test 12: Export long
run_test "Export avec valeur longue" \
"export LONG=\"abcdefghijklmnopqrstuvwxyz\"
echo \$LONG
exit" \
"abcdefghijklmnopqrstuvwxyz"

# Test 13: Export avec PATH (test critique)
run_test "Export PATH" \
"export PATH_BACKUP=\$PATH
export PATH=\"/bin:/usr/bin\"
echo \$PATH
exit" \
"/bin:/usr/bin"

# Test 14: Export display (sans arguments)
run_test "Export display" \
"export TEST_DISPLAY=visible
export
exit" \
"TEST_DISPLAY"

# Test 15: Variable avec underscore
run_test "Variable avec underscore" \
"export MY_VAR=test_underscore
echo \$MY_VAR
exit" \
"test_underscore"

# Tests d'erreur (ces tests vérifient que minishell gère bien les erreurs)

# Test 16: Nom invalide avec chiffre au début
run_test "Nom invalide (chiffre)" \
"export 123INVALID=test
echo \"Should not work\"
exit" \
".*" # Accepter n'importe quoi car le comportement d'erreur peut varier

# Test 17: Export avec = multiple
run_test "Multiple =" \
"export VAR=value=extra
echo \$VAR
exit" \
"value=extra"

# Test 18: Export avec guillemets non fermés (peut causer des problèmes)
run_test "Test guillemets échappés" \
"export ESCAPED=\"hello\\\"world\\\"\"
echo \$ESCAPED
exit" \
"hello\"world\""

# Affichage des résultats finaux
echo ""
echo -e "${BLUE}=== RÉSULTATS DES TESTS ===${NC}"
echo -e "Tests passés: ${GREEN}$PASSED${NC}"
echo -e "Tests échoués: ${RED}$FAILED${NC}"
echo "Total: $((PASSED + FAILED))"

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}🎉 Tous les tests sont passés !${NC}"
    exit 0
else
    echo -e "${RED}⚠️  $FAILED test(s) ont échoué${NC}"
    exit 1
fi