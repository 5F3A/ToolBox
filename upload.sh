#!/bin/bash
# ============================================
#  ToolBox 一键上传脚本（git-flow 版）
# ============================================

set -e

REPO_DIR="/mnt/d/visual studio data/toolbox"
cd "$REPO_DIR"

# ===== 配置区 =====
GITHUB_REPO="git@github.com:5F3A/ToolBox.git"
GITEE_REPO="git@gitee.com:yuan-mingtai/tool-box.git"
DEFAULT_BRANCH="develop"
# ==================

echo "============================================"
echo "  ToolBox 一键上传脚本"
echo "  目录: $REPO_DIR"
echo "============================================"
echo ""

# 1. 初始化 git
if [ ! -d ".git" ]; then
    echo "[1/7] 初始化 git 仓库..."
    git init
    git checkout -b "$DEFAULT_BRANCH" 2>/dev/null || git branch -M "$DEFAULT_BRANCH"
else
    echo "[1/7] git 仓库已存在"
fi

# 2. 配置 remote
echo "[2/7] 配置 remote..."
if ! git remote | grep -q "^origin$"; then
    git remote add origin "$GITHUB_REPO"
else
    git remote set-url origin "$GITHUB_REPO"
fi
echo "  origin → $GITHUB_REPO"

if ! git remote | grep -q "^gitee$"; then
    git remote add gitee "$GITEE_REPO"
else
    git remote set-url gitee "$GITEE_REPO"
fi
echo "  gitee  → $GITEE_REPO"

# 3. .gitignore
echo "[3/7] 检查 .gitignore..."
if [ ! -f ".gitignore" ]; then
    cat > .gitignore << 'EOF'
# 编译产物
*.obj
*.exe
*.pdb
*.ilk
*.exp
*.lib

# 中间目录
bin/
obj/
.vs/
x64/
Debug/
Release/

# 用户设置
*.user
*.suo
*.vcxproj.user

# 系统
Thumbs.db
.DS_Store
desktop.ini

# 日志
*.log

# 临时
~$*
*.tmp
EOF
    echo "  已创建 .gitignore"
else
    echo "  .gitignore 已存在"
fi

# 4. 首次拉取远端内容（处理 GitHub 的初始提交）
echo "[4/7] 检查远端是否有内容..."
if git ls-remote --exit-code origin >/dev/null 2>&1; then
    echo "  origin 有内容，尝试合并..."
    git fetch origin "$DEFAULT_BRANCH" 2>/dev/null || git fetch origin master 2>/dev/null || true
    REMOTE_BRANCH=$(git branch -r | grep -E "origin/(master|main|$DEFAULT_BRANCH)" | head -1 | sed 's/.*origin\///' | tr -d ' ')
    if [ -n "$REMOTE_BRANCH" ]; then
        git merge "origin/$REMOTE_BRANCH" --allow-unrelated-histories --no-edit 2>/dev/null || true
        echo "  已合并 origin/$REMOTE_BRANCH"
    fi
else
    echo "  origin 为空，首次推送"
fi

if git ls-remote --exit-code gitee >/dev/null 2>&1; then
    echo "  gitee 有内容，尝试合并..."
    git fetch gitee "$DEFAULT_BRANCH" 2>/dev/null || git fetch gitee master 2>/dev/null || true
    REMOTE_BRANCH=$(git branch -r | grep -E "gitee/(master|main|$DEFAULT_BRANCH)" | head -1 | sed 's/.*gitee\///' | tr -d ' ')
    if [ -n "$REMOTE_BRANCH" ]; then
        git merge "gitee/$REMOTE_BRANCH" --allow-unrelated-histories --no-edit 2>/dev/null || true
        echo "  已合并 gitee/$REMOTE_BRANCH"
    fi
fi

# 5. 添加 & 提交
echo "[5/7] 添加文件..."
git add -A
git status --short

echo ""
echo "[6/7] 提交..."
COMMIT_MSG="${1:-update: $(date '+%Y-%m-%d %H:%M:%S')}"
git commit -m "$COMMIT_MSG" || echo "  没有变化，跳过"

# 6. 推送
echo ""
echo "[7/7] 推送..."
CURRENT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
echo "  当前分支: $CURRENT_BRANCH"
echo ""

echo "  → GitHub (origin/$CURRENT_BRANCH)..."
git push -u origin "$CURRENT_BRANCH" 2>&1 || echo "  [X] GitHub 推送失败"

echo ""
echo "  → Gitee (gitee/$CURRENT_BRANCH)..."
git push -u gitee "$CURRENT_BRANCH" 2>&1 || echo "  [X] Gitee 推送失败"

echo ""
echo "============================================"
echo "  ✅ 上传完成"
echo "============================================"
echo "  GitHub: https://github.com/5F3A/ToolBox"
echo "  Gitee:  https://gitee.com/yuan-mingtai/tool-box"
