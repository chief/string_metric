# coding: utf-8

module StringMetric
  module Levenshtein
    class IterativeWithTwoMatrixRowsOptimized
      def self.distance(from, to, options = {})
        return 0 if from == to
        return to.size if from.size.zero?
        return from.size if to.size.zero?

        max_distance      = options[:max_distance]
        insertion_cost    = options[:insertion_cost]    || 1
        deletion_cost     = options[:deletion_cost]     || 1
        substitution_cost = options[:substitution_cost] || 1

        m = from.length
        n = to.length

        from = from.codepoints.to_a
        to = to.codepoints.to_a

        v0 = (0..m).to_a
        v1 = []
        x = 0

        n.times do |i|
          x = v1[0] = i + 1

          sub_cell = v0[0]

          m.times do |j|
            cost = (from[j] == to[i]) ? 0 : substitution_cost

            ins_cell = v0[j + 1]

            x = [x + deletion_cost,         # deletion
                 ins_cell + insertion_cost, # insertion
                 sub_cell + cost            # substitution
                ].sort![0]

            v1[j + 1] = x

            sub_cell = ins_cell
          end

          break if max_distance && v0.min > max_distance

          v0 = v1.dup
        end

        if max_distance && x > max_distance
          max_distance
        else
          x
        end
      end
    end
  end
end
