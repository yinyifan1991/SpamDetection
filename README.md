# SpamDetection
Spam reviewer detection
## Abstract
This work proposes a graph mining angle to the problem by modeling rating relations as edges and triangles. A spam reviewer grading mechanism based on counting triangle motifs is proposed. It works on the Amazon rating dataset which provides rating records and co-bought relations between products. The intuition is that spam reviewers usually review unrelated products and give abnormal ratings. We also apply k-truss decomposition on rating data to find features of dense subgraphs helping identify behaviors of reviewers. Experiments on several Amazon rating datasets demonstrate the effectiveness of the proposed models which reflect abnormal behaviors of reviewers and automatically shows reviewers with high possibility to be spammers.
