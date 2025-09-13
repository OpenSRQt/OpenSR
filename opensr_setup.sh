#!/bin/bash

ASSETS="${ASSETS:-$(realpath "../SR2Image")}"
LNG="${LNG:-Rus}"
TARGET="${TARGET:-all}"
DATCONVERT="build/tools/DATTools/opensr-dat-convert"
DATJSON="build/tools/DATTools/opensr-dat-json"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

help_function()
{
   echo ""
   echo "Usage: $0 [-a assets_path] [-l language] [-t target]"
   echo -e "\t-a Path to SR2 assets directory (default: ../SR2Image)"
   echo -e "\t-l Language: Rus or Eng (default: Rus)"
   echo -e "\t-t Target: all, data, datfiles, tools, demo (default: all)"
   echo ""
   exit 1
}

# Parse command line options
while getopts "a:l:t:h" opt
do
   case "$opt" in
      a ) ASSETS="$OPTARG" ;;
      l ) LNG="$OPTARG" ;;
      t ) TARGET="$OPTARG" ;;
      h ) help_function ;;
      ? ) help_function ;;
   esac
done

print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
    help_function
}

verify_assets() {
    if [ ! -d "$ASSETS" ]; then
        print_error "Assets directory not found: $ASSETS"
    fi
}

verify_lang() {
    if [ "$LNG" != "Rus" ] && [ "$LNG" != "Eng" ]; then
        print_error "Unsupported language '$LNG'. Supported values: Rus, Eng"
    fi
}

check_dependencies() {
    if [ ! -f "$DATCONVERT" ]; then
        print_error "DATCONVERT not found: $DATCONVERT"
    fi
    
    if [ ! -f "$DATJSON" ]; then
        print_error "DATJSON not found: $DATJSON"
    fi
}

create_data_links() {
    verify_assets

    print_info "Creating symlinks to .pkg files"
    cd data && find "$ASSETS/DATA" -iname '*.pkg' -exec ln -sfv {} \; && cd .. || print_error "Failed to create pkg symlinks"
    
    print_info "Creating symlink to Music directory"
    cd data && ln -sf "$ASSETS/Music/" . && cd .. || print_warning "Failed to create Music directory symlink"

    print_info "Symlinks created successfully!"
}

process_dat_files() {
    verify_assets
    verify_lang
    
    local lang_file="$LNG"
    local lang_file_lower=$(echo "$LNG" | tr '[:upper:]' '[:lower:]')
    local fallback=""

    if [ "$LNG" = "Rus" ]; then
        fallback="Eng"
    else
        fallback="Rus"
    fi

    # Check if main language file exists, use fallback if not
    if [ ! -f "$ASSETS/CFG/$lang_file.dat" ]; then
        print_warning "$lang_file.dat not found, trying fallback: $fallback.dat"
        lang_file="$fallback"
        lang_file_lower=$(echo "$fallback" | tr '[:upper:]' '[:lower:]')
        
        # Check if fallback file exists
        if [ ! -f "$ASSETS/CFG/$lang_file.dat" ]; then
            print_error "Neither $LNG.dat nor $fallback.dat found in $ASSETS/CFG/"
        fi
    fi
    
    print_info "Decoding DAT files"
    "$DATCONVERT" hd "$ASSETS/CFG/CacheData.dat" "data/CacheData.dat" || print_error "Failed to decode CacheData.dat"
    "$DATCONVERT" d "$ASSETS/CFG/$lang_file.dat" "data/$lang_file.dat" || print_error "Failed to decode $lang_file_lower.dat"
    "$DATCONVERT" d "$ASSETS/CFG/Main.dat" "data/main.dat" || print_error "Failed to decode Main.dat"
    
    print_info "Converting DAT files to JSON"
    export LD_LIBRARY_PATH="build/Ranger"
    "$DATJSON" d2j "data/main.dat" "data/main.json" || print_error "Failed to convert main.dat to JSON"
    "$DATJSON" d2j "data/CacheData.dat" "data/CacheData.json" || print_error "Failed to convert CacheData.dat to JSON"
    "$DATJSON" d2j "data/$lang_file.dat" "data/$lang_file_lower.json" || print_error "Failed to convert $lang_file.dat to JSON"

    print_info "DAT files processed successfully!"
}

setup_demo() {
    print_info "Setting up symlinks required for demo"
    
    # World library
    ln -sf "build/World/libworld.so" || print_warning "Failed to create libworld.so symlink"
    
    mkdir -p "imageformats" && \
    cd "imageformats" && \
    ln -sf "../build/ImagePlugin/libQtOpenSRImagePlugin.so" . && \
    cd .. || print_warning "Failed to setup image formats for demo"

    print_info "Demo setup completed successfully!"
}

setup_tools() {
    print_info "Setting up symlinks required for resource viewer"

    mkdir -p "build/tools/ResourceViewer/imageformats" && \
    cd "build/tools/ResourceViewer/imageformats" && \
    ln -sf "../../../ImagePlugin/libQtOpenSRImagePlugin.so" . && \
    cd ../../../../ || print_warning "Failed to setup image formats for resource viewer"

    print_info "Tools setup completed successfully!"
}

setup_spix() {
    INSTALL_DIR="$(pwd)/deps-install"
    print_info "Local install directory for Spix and its dependencies is set to: $INSTALL_DIR"

    mkdir -p "deps" && cd ./deps
    print_info "Cloning Spix repository"
    git clone --recursive https://github.com/faaxm/spix.git && cd .. || print_error "Failed to clone Spix repository"
    
    print_info "Building and installing Spix dependencies"
    chmod +x ./deps/spix/ci/install-deps.sh && \
    ./deps/spix/ci/install-deps.sh local_build || print_error "Failed to build and install Spix dependencies"

    print_info "Building and installing Spix"
    cd ./deps/spix && mkdir -p "build" && cd ./build && \
    cmake -DSPIX_QT_MAJOR=6 -DSPIX_BUILD_EXAMPLES=OFF -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR .. && \
    cmake --build . && cmake --install . && cd ../../../ || print_error "Failed to build and install"

    print_info "Copying AnyRPC module to: './cmake/modules'"
    mkdir -p ./cmake/modules && cp -r ./deps/spix/cmake/modules/FindAnyRPC.cmake ./cmake/modules/ || print_error "Failed to copy module file"

    print_info "Spix setup completed successfully!"
}

main() {
    print_info "Starting setup process with ASSETS=$ASSETS, LNG=$LNG"
    
    check_dependencies
    create_data_links
    process_dat_files
    setup_tools
    setup_demo

    print_info "Setup completed successfully!"
}

case "$TARGET" in
    "data")
        create_data_links
        ;;
    "datfiles")
        check_dependencies
        process_dat_files
        ;;
    "tools")
        setup_tools
        ;;
    "demo")
        setup_demo
        ;;
    "spix")
        setup_spix
        ;;
    "all")
        main
        ;;
    *)
        print_error "Unknown target '$TARGET'"
        ;;
esac