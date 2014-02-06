CREATE_DIR:=$(shell if [ ! -e target/objects ]; then mkdir -p target/objects; echo "mkdir -p target/objects"; fi)

# This recreates the Makeppfile responsible for building the objects
target/objects/Makeppfile: target/objects/Makeppfile.auto
    @if [ ! -e target/objects ]; then mkdir -p target/objects; echo "mkdir -p target/objects"; fi
    @if [ -e ${output} ] && diff --brief ${input} ${output} >/dev/null ; then \
        true; \
    else \
        echo "cp ${input} ${output}"; \
        cp ${input} ${output}; \
    fi

target/objects/Makeppfile.auto: FORCE scripts/generateMakeppfileForObjects.sh scripts/MakeppfileForObjectsTemplate
    @if [ ! -e target/objects ]; then mkdir -p target/objects; echo "mkdir -p target/objects"; fi
    @./scripts/generateMakeppfileForObjects.sh > ${output}
    @if [ -e target/objects/Makeppfile ] && diff --brief ${output} target/objects/Makeppfile >/dev/null ; then \
        true; \
    else \
        echo "./scripts/generateMakeppfileForObjects.sh > ${output}"; \        
    fi
