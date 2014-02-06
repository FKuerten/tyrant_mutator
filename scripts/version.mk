# This is responsible for creating a version header
target/generated/version.h++: target/generated/version.h++.auto
    @if [ ! -e target/generated ]; then mkdir -p target/generated; echo "mkdir -p target/generated"; fi
    @if [ -e ${output} ] && diff --brief ${input} ${output} >/dev/null ; then \
        true; \
    else \
        echo "cp ${input} ${output}"; \
        cp ${input} ${output}; \
    fi

target/generated/version.h++.auto: FORCE scripts/generateVersionH++.sh
    @if [ ! -e target/generated ]; then mkdir -p target/generated; echo "mkdir -p target/generated"; fi
    @./scripts/generateVersionH++.sh > ${output}
    @if [ -e target/generated/version.h++ ] && diff --brief ${output} target/generated/version.h++ >/dev/null ; then \
        true; \
    else \
        echo "./scripts/generateVersionH++.sh > ${output}"; \        
    fi
