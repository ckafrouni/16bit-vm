#[derive(Debug)]
enum Mov {
    ImmToReg(u32, u8),
    RegToReg(u8, u8),
    RegToMem(u8, u32),
    MemToReg(u32, u8),
}

#[derive(Debug)]
enum AstNode {
    Mov(Mov),
}

#[derive(Debug)]
struct Ast {
    nodes: Vec<AstNode>,
}

fn main() {
    let ast = Ast {
        nodes: vec![
            AstNode::Mov(Mov::ImmToReg(0x1234, 1)),
            AstNode::Mov(Mov::RegToReg(1, 2)),
            AstNode::Mov(Mov::RegToMem(2, 0x1000)),
            AstNode::Mov(Mov::MemToReg(0x1000, 3)),
        ],
    };

    println!("{:?}", ast);

    let mut code = Vec::new();
    for node in ast.nodes {
        match node {
            AstNode::Mov(mov) => match mov {
                Mov::ImmToReg(imm, reg) => {
                    code.push(0x01);
                    code.push(reg);
                    code.append(&mut imm.to_be_bytes().to_vec());
                }
                Mov::RegToReg(src, dst) => {
                    code.push(0x02);
                    code.push(src);
                    code.push(dst);
                }
                Mov::RegToMem(src, mem) => {
                    code.push(0x03);
                    code.push(src);
                    code.append(&mut mem.to_be_bytes().to_vec());
                }
                Mov::MemToReg(mem, dst) => {
                    code.push(0x04);
                    code.append(&mut mem.to_be_bytes().to_vec());
                    code.push(dst);
                }
            },
        }
    }

    println!("{:?}", code);
}
