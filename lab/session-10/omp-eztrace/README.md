# Quickstart

**First, prepare your environment*:

Install the eztrace-build for SuperMUC in your SuperMUC home directory:

```bash
$ mkdir ${HOME}/opt
$ cp eztrace-supermuc-homeinst.tar.gz ${HOME}/opt
$ (cd $HOME/opt && tar -xf eztrace-supermuc-homeinst.tar.gz)
```

Then, once **in every new terminal session**:

```bash
$ source ${HOME}/opt/eztrace/env.sh
```


**Compile with eztrace instrumentation**:

(Already defined in Makefile)

```bash
$ eztrace_cc $CC <as usual> -fopenmp -lm
```

**Run and trace application**:

```bash
$ eztrace -t omp ./my_openmp_application
```

**Postmortem analysis**:

Profiling stats:

```bash
$ eztrace_stats myusername_eztrace_log_rank_1
```

Visualize trace:

```bash
$ eztrace_convert myusername_eztrace_log_rank_1
$ vite eztrace_output.trace
```

