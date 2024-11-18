# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sleep-and-preempt) begin
(sleep-and-preempt) end
EOF
pass;
