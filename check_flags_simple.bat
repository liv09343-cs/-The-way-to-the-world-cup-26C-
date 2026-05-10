@echo off
setlocal enabledelayedexpansion

echo ============================================
echo     2026 World Cup Flags Check Tool
echo ============================================
echo.

set "FLAGS_DIR=flags"
set "MISSING_COUNT=0"

echo Required flag files:
echo -------------------

set "TEAMS=墨西哥 南非 韩国 捷克 加拿大 波黑 卡塔尔 瑞士 巴西 摩洛哥 海地 苏格兰 美国 巴拉圭 澳大利亚 土耳其 德国 库拉索 科特迪瓦 厄瓜多尔 荷兰 日本 瑞典 突尼斯 比利时 埃及 伊朗 新西兰 西班牙 佛得角 沙特阿拉伯 乌拉圭 法国 塞内加尔 伊拉克 挪威 阿根廷 阿尔及利亚 奥地利 约旦 葡萄牙 民主刚果 乌兹别克斯坦 哥伦比亚 英格兰 克罗地亚 加纳 巴拿马"

for %%t in (%TEAMS%) do (
    if exist "%FLAGS_DIR%\flag_%%t.png" (
        echo [OK] flag_%%t.png
    ) else (
        echo [MISSING] flag_%%t.png
        set /a MISSING_COUNT+=1
    )
)

echo.
echo ============================================
if !MISSING_COUNT! equ 0 (
    echo All flag files are present!
) else (
    echo Missing !MISSING_COUNT! flag files, please check
)
echo ============================================

pause