#!/bin/sh
# Refuse any commit that would track a garment, avatar, pattern, render or studio path.
cd "$(dirname "$0")/.." || exit 1
cat > .git/hooks/pre-commit <<'HOOK'
#!/bin/sh
exec uv run --quiet --with pytest pytest -q tests/test_no_assets.py
HOOK
chmod +x .git/hooks/pre-commit
echo "pre-commit guard installed"
